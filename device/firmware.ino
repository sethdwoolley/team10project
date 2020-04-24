/**
 * @file firmware.ino
 * @author Oliver Newman (oliver@olivernewman.co.uk)
 * @brief 
 * @version 0.0.4
 * @date 2020-04-10
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>

#include <FlashStorage.h>
#include <Scheduler.h>

#include <RTCZero.h>

#include <DHT.h>

#include "firmware_config.h"    // Preprocessor definitions relating to debugging etc

#include "src/settings.h"

#include "src/telemetry/telemetry.h"
#include "src/temperature/temperature.h"
#include "src/humidity/humidity.h"
#include "src/lux/lux.h"
#include "src/rgb/rgb.h"
#include "src/moisture/moisture.h"

#include "src/screen/screen.h"

#include "src/menu/menu_state.h"

#include "src/menu/menu.h"
#include "src/menu/menu_startup.h"
#include "src/menu/menu_off.h"
#include "src/menu/menu_main.h"
#include "src/menu/menu_telemetry.h"
#include "src/menu/menu_settings.h"
#include "src/menu/menu_about.h"
#include "src/menu/menu_calibration.h"

#include "secrets.h" // Includes sensitive hardcoded values such as wifi SSID and password. Used for testing.

/*
 * Arduino defines the main function within its core library. It calls the 
 * setup function below once, and runs the loop function indefinitely. The
 * declarations below are therefore essentially global variables, however, by 
 * including them in this file beflow the include statements, we essentially
 * limit their access to the main function. 
 */

FlashStorage(settings_store, Settings);
FlashStorage(moisture_store, Moisture_range);

WiFiClient wifiClient;
HttpClient httpClient(wifiClient, SERVER_URL);

RTCZero rtc;
u_long lastSync;

Settings settings = {
    true,
//      name                value   unit    rangeIndex  range                                                           rangeSize
    {   "Button Response",  5,      "",     6,          { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },                              10  }, 
    {   "Timeout Interval", 10,     "s",    1,          { 5, 10, 15, 20, 30, 45, 60 },                                  7   }, 
    {   "Publish Interval", 5,      "s",    2,          { 1, 2, 5, 10, 15, 20, 30, 45, 60, 120, 180, 240, 300, 600 },   14  } 
};

typedef struct 
{
    const char *ssid;
    const char *pass;

} Wifi_settings;

Wifi_settings wifiSettings = { WIFI_SSID, WIFI_PASS };
int wifi_status = WL_IDLE_STATUS;


Moisture_range moistureRange = {true, 800, 375};

enum class button
{
    TOP_LEFT,
    TOP_RIGHT,
    BOTTOM_LEFT,
    BOTTOM_RIGHT
};

DHT                 dht(2, 22);

TEMPERATURE         temperature(&dht);
HUMIDITY            humidity(&dht);
LUX                 lux(A2);
MOISTURE            moisture(A1, moistureRange);

RGB                 rgb(3, 4, 5);

MENU_STARTUP        menuStartup;
MENU_OFF            menuOff;
MENU_MAIN           menuMain;
MENU_TELEMETRY      menuTelemetry(&temperature, &humidity, &lux, &moisture);
MENU_SETTINGS       menuSettings(&settings);
MENU_ABOUT          menuAbout;
MENU_CALIBRATION    menuCalibration(&moisture);

MENU                *currentMenu;

SCREEN              screen;

menu_state          menuState;

int buttonPins[] = { 10, 9 ,7, 8 };
int buttonStates[4] = { 0 };

long timeoutMillis = 0;
long publishMillis = 0;
long buttonMillis = 0;

bool buttonPressed = false;

void setup()
{
    Serial.begin(9600);
    #ifdef SERIAL_WAIT
    while(!Serial);
    #endif

    screen.init();
    menuState = menu_state::MENU_STARTUP;
    
    menuStartup.setMaxProgress(20);
    menuStartup.setLoadText("Initialising WiFi");
    menuStartup.draw(&screen);
    wifi_setup();
    menuStartup.setLoadText("Initialising RTC");
    menuStartup.progress(2);
    menuStartup.draw(&screen);
    rtc.begin();
    menuStartup.setLoadText("Syncing RTC");
    menuStartup.progress(2);
    menuStartup.draw(&screen);
    rtc_sync();

    menuStartup.setLoadText("Initialising Buttons");
    menuStartup.progress(2);
    menuStartup.draw(&screen);
    for (int i = 0; i < 4; i++)
        pinMode(buttonPins[i], INPUT);

    menuStartup.setLoadText("Initialising Sensors");
    menuStartup.progress(2);
    menuStartup.draw(&screen);
    dht.begin();
    rgb.init();
    lux.init();
    moisture.init();

    menuStartup.setLoadText("Complete!");
    menuStartup.progress(10);
    menuStartup.draw(&screen);
    delay(1000);

    menuState = menu_state::MENU_OFF;
    currentMenu = &menuOff;

    currentMenu->draw(&screen);
    timeoutMillis = millis();
}

void loop()
{
    // Sync the rtc every 24 hours
    if (rtc.getEpoch() - lastSync > RTC_SYNC)
        rtc_sync();

    // Publish telemetry loop
    if (millis() - publishMillis > settings.publishInterval.value * 1000)
    {
        publish_telemetry();
        if (menuState == menu_state::MENU_TELEMETRY) currentMenu->draw(&screen);
        publishMillis = millis();
    }

    // Button control loop
    if (!buttonPressed)
    {
        for (int i = 0; i < 4; i++)
        {
            buttonStates[i] = digitalRead(buttonPins[i]);
            if (buttonStates[i])
            {
                buttonControl((button)i);
                
                buttonPressed = true;
                buttonMillis = millis();
                timeoutMillis = millis();
            }
        }
    } 
    else if (millis() - buttonMillis > 1400 / settings.buttonSensitivity.value)
        buttonPressed = false;

    // Menu timeout loop
    // If on the telemetry menu, increase the timeout value
    int telemetryTimeoutMultiplier = 10;
    int timeoutInterval = menuState == menu_state::MENU_TELEMETRY ? 
        settings.timeoutInterval.value  * telemetryTimeoutMultiplier : 
        settings.timeoutInterval.value;

    // If screen is not off, check whether the timeout interval has been 
    // passed, and if so, turn off screen
    if (menuState != menu_state::MENU_OFF && 
        menuState != menu_state::CALIBRATION && 
        millis() - timeoutMillis > timeoutInterval * 1000)
    {
        menuState = menu_state::MENU_OFF;
        currentMenu = &menuOff;
        
        menuMain.reset();
    }

    currentMenu->draw(&screen);
}

void buttonControl(button b)
{   
    menu_state newState = menu_state::NO_CHANGE;
    switch (b)
    {
        case button::TOP_LEFT:
            newState = currentMenu->buttonTL();
            break;
        case button::BOTTOM_LEFT:
            newState = currentMenu->buttonBL();
            break;
        case button::TOP_RIGHT:
            newState = currentMenu->buttonTR();
            break;
        case button::BOTTOM_RIGHT:
            newState = currentMenu->buttonBR();
            break;
        default:
            break;
    }


    if (newState != menu_state::NO_CHANGE)
    {
        
        switch (newState)
        {
            case menu_state::MENU_MAIN:
                currentMenu = &menuMain;
                break;
            case menu_state::MENU_TELEMETRY:
                currentMenu = &menuTelemetry;
                currentMenu->reset();
                break;
            case menu_state::MENU_SETTINGS:
                currentMenu = &menuSettings;
                // If we're coming back from the calibration state, we don't 
                // want to reset the menu.
                if (menuState != menu_state::CALIBRATION)
                    currentMenu->reset();
                break;
            case menu_state::MENU_ABOUT:
                currentMenu = &menuAbout;
                currentMenu->reset();
                break;
            case menu_state::CALIBRATION:
                currentMenu = &menuCalibration;
                currentMenu->reset();
                break;
            default:
                break;
        }

        menuState = newState;
    }
}

void wifi_setup()
{
    while (wifi_status != WL_CONNECTED)
    {
        #ifdef SERIAL_PRINT
        Serial.print("Attempting to connect to SSID: ");
        Serial.println(wifiSettings.ssid);
        #endif

        wifi_status = WiFi.begin(wifiSettings.ssid, wifiSettings.pass);
        u_long timeCount = millis();
        u_long progressTime = 1000;
        int count = 0;
        while (millis() - count < 10000)
        {
            if (count < 10 && millis() - count > progressTime)
            {
                count++;
                menuStartup.progress();
                menuStartup.draw(&screen);
                progressTime += 1000;
            }
        }
    }
}

void rtc_sync()
{
    u_long epoch = 0;

    while (epoch == 0)
    {
        #ifdef SERIAL_PRINT
        // Serial.print("Wifi time: "); Serial.println(WiFi.getTime());
        #endif
        epoch = WiFi.getTime();
    }
    
    #ifdef SERIAL_PRINT
    Serial.print("epoch received: "); Serial.println(epoch);
    #endif

    rtc.setEpoch(epoch);
    lastSync = epoch;

    #ifdef SERIAL_PRINT
    Serial.print("rtc reading: ");
    Serial.println(rtc.getEpoch());
    Serial.print(rtc.getDay());
    Serial.print("/");
    Serial.print(rtc.getMonth());
    Serial.print("/");
    Serial.print(rtc.getYear());
    Serial.print(" ");
    Serial.print(rtc.getHours());
    Serial.print(":");
    Serial.print(rtc.getMinutes());
    Serial.print(":");
    Serial.println(rtc.getSeconds());
    #endif
}

void publish_telemetry()
{
    String contentType = "application/json";

    float t = temperature.getReading();
    float h = humidity.getReading();
    float l = lux.getReading();
    float m = moisture.getReading();
    ulong e = rtc.getEpoch();

    String telemetry = "{\"ts\":" + String(e) + ",";
    telemetry += "\"data\":[";
    telemetry += "{\"name\":\"temperature\",\"value\":" + String(t,1) + "},";
    telemetry += "{\"name\":\"humidity\",\"value\":" + String(h,1) + "},";
    telemetry += "{\"name\":\"lux\",\"value\":" + String(l,0) + "},";
    telemetry += "{\"name\":\"moisture\",\"value\":" + String(m,1) + "}";
    telemetry += "]}";

     httpClient.post(path, contentType, telemetry);
    Serial.println(telemetry);
}