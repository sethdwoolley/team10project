#include "menu_telemetry.h"

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "../screen/screen.h"
#include "menu_state.h"

MENU_TELEMETRY::MENU_TELEMETRY(TEMPERATURE *t, HUMIDITY *h, LUX *l, MOISTURE *m)
    : MENU("Telemetry", 4), m_temp(t), m_humidity(h), m_lux(l), m_moisture(m)
{
    setTelemetry();
}

void MENU_TELEMETRY::setTelemetry()
{
    switch(m_selected)
    {
        case 0:
            m_telemetry = m_temp;
            break;
        case 1:
            m_telemetry = m_humidity;
            break;
        case 2:
            m_telemetry = m_lux;
            break;
        case 3:
            m_telemetry = m_moisture;
            break;
        default:
            m_telemetry = m_temp;
            break;
    }
}

void MENU_TELEMETRY::draw(SCREEN *screen)
{

    Adafruit_SSD1306 *display = screen->getDisplay();

    String header = m_telemetry->getName();
    String reading = String(m_telemetry->getLastReading(), m_telemetry->getDecPlaces());
    String unit = m_telemetry->getUnits();

    display->clearDisplay();

    int textSize = 1;
    int optionSize = 2;
    display->setTextSize(optionSize);
    display->setTextColor(SSD1306_WHITE);

    screen->drawControls(false);

    display->setTextSize(textSize);
    
    display->setCursor(screen->centerText(m_title, textSize),screen->getYPositionFromTop(0,textSize));
    display->println(m_title);

    display->setCursor(screen->centerText(header, textSize),screen->getYPositionFromTop(1,textSize));
    display->println(header);

    display->setTextSize(optionSize);

    display->setCursor(screen->centerText(reading, optionSize) - (unit.length() * 3), screen->getYPositionFromTop(2,textSize));
    display->print(reading);

    display->setCursor(display->getCursorX(), screen->getYPositionFromTop(3,textSize));
    display->setTextSize(textSize);
    display->println(unit);

    display->display();
}

void MENU_TELEMETRY::reset()
{
    m_selected = 0;
    setTelemetry();
}

menu_state MENU_TELEMETRY::buttonTL()
{
    m_selected = m_selected > 0 ? m_selected -1 : m_menuSize -1;
    setTelemetry();
    return menu_state::NO_CHANGE;
}

menu_state MENU_TELEMETRY::buttonBL()
{
    m_selected = ++m_selected % m_menuSize;
    setTelemetry();
    return menu_state::NO_CHANGE;
}

menu_state MENU_TELEMETRY::buttonTR()
{
    return menu_state::NO_CHANGE;
}

menu_state MENU_TELEMETRY::buttonBR()
{
    return menu_state::MENU_MAIN;
}

