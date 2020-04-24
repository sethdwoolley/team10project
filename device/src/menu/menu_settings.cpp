#include "menu_settings.h"

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "../screen/screen.h"
#include "../settings.h"
#include "menu_state.h"

#include "../moisture/moisture.h"

MENU_SETTINGS::MENU_SETTINGS(Settings *settings)
    : MENU("Settings", 5), m_settings(settings), 
    m_state(settings_state::SETTING)
{
    m_item = &m_settings->publishInterval;
}

void MENU_SETTINGS::draw(SCREEN *screen)
{
    switch(m_state)
    {
        case settings_state::SETTING:
            drawDefault(screen);
            break;
        case settings_state::CONFIG:
            drawConfig(screen);
            break;
        case settings_state::MOISTURE:
            drawMoisture(screen);
            break;
        default:
            break;
    }
}

void MENU_SETTINGS::drawDefault(SCREEN *screen)
{
    Adafruit_SSD1306 *display = screen->getDisplay();

    String header = m_item->name;
    String value = String(m_item->value);
    String unit = m_item->unit;

    int textSize = 1;
    int optionSize = 2;

    display->clearDisplay();
    screen->drawControls();

    display->setTextColor(SSD1306_WHITE);
    display->setTextSize(textSize);

    display->setCursor(screen->centerText(m_title, textSize),screen->getYPositionFromTop(0,textSize));
    display->println(m_title);

    display->setCursor(screen->centerText(header, textSize),screen->getYPositionFromTop(1,textSize));
    display->println(header);

    display->setTextSize(optionSize);

    display->setCursor(screen->centerText(value, optionSize) - (unit.length() * 3), screen->getYPositionFromTop(2,textSize));
    display->print(value);

    display->setTextSize(textSize);

    display->setCursor(display->getCursorX(), screen->getYPositionFromTop(3,textSize));
    display->println(unit);

    display->display();

}

void MENU_SETTINGS::drawConfig(SCREEN *screen)
{
    Adafruit_SSD1306 *display = screen->getDisplay();

    String header = "Config Server";
    String button = "START";

    int textSize = 1;
    int optionSize = 2;

    display->clearDisplay();
    screen->drawControls();

    display->setTextColor(SSD1306_WHITE);
    display->setTextSize(textSize);

    display->setCursor(screen->centerText(m_title, textSize),screen->getYPositionFromTop(0,textSize));
    display->println(m_title);

    display->setCursor(screen->centerText(header, textSize),screen->getYPositionFromTop(1,textSize));
    display->println(header);

    display->fillRoundRect(screen->centerText(button, optionSize), 
                        screen->getYPositionFromTop(2,textSize) + 2, 
                        button.length() * 6 * optionSize, 
                        8 + 4, 
                        5, 
                        SSD1306_WHITE);

    display->setTextColor(SSD1306_BLACK);
    display->setTextSize(textSize);

    display->setCursor(screen->centerText(button, textSize),screen->getYPositionFromTop(3,textSize) - 4 );
    display->println(button);

    display->display();
}

void MENU_SETTINGS::drawMoisture(SCREEN *screen)
{
    Adafruit_SSD1306 *display = screen->getDisplay();

    String header = "Calibrate Sensor";
    String button = "START";

    int textSize = 1;
    int optionSize = 2;

    display->clearDisplay();
    screen->drawControls();

    display->setTextColor(SSD1306_WHITE);
    display->setTextSize(textSize);

    display->setCursor(screen->centerText(m_title, textSize),screen->getYPositionFromTop(0,textSize));
    display->println(m_title);

    display->setCursor(screen->centerText(header, textSize),screen->getYPositionFromTop(1,textSize));
    display->println(header);

    display->fillRoundRect(screen->centerText(button, optionSize), 
                        screen->getYPositionFromTop(2,textSize) + 2, 
                        button.length() * 6 * optionSize, 
                        8 + 4, 
                        5, 
                        SSD1306_WHITE);

    display->setTextColor(SSD1306_BLACK);
    display->setTextSize(textSize);

    display->setCursor(screen->centerText(button, textSize),screen->getYPositionFromTop(3,textSize) - 4 );
    display->println(button);

    display->display();
}

void MENU_SETTINGS::reset()
{
    m_selected = 0;
    m_state = settings_state::SETTING;
}

void MENU_SETTINGS::setState()
{
    switch(m_selected)
    {
        case 0:
            m_state = settings_state::SETTING;
            m_item = &m_settings->publishInterval;
            break;
        case 1:
            m_state = settings_state::SETTING;
            m_item = &m_settings->timeoutInterval;
            break;
        case 2:
            m_state = settings_state::SETTING;
            m_item = &m_settings->buttonSensitivity;
            break;
        case 3:
            m_state = settings_state::CONFIG;
            break;
        case 4:
            m_state = settings_state::MOISTURE;
            break;
        default:
            m_state = settings_state::SETTING;
            m_selected = 0;
            break;
    }
}

menu_state MENU_SETTINGS::buttonTL()
{
    m_selected = m_selected > 0 ? m_selected -1 : m_menuSize -1;
    setState();

    return menu_state::NO_CHANGE;
}

menu_state MENU_SETTINGS::buttonBL()
{
    m_selected = ++m_selected % m_menuSize;
    setState();

    return menu_state::NO_CHANGE;
}

menu_state MENU_SETTINGS::buttonTR()
{
    switch (m_state)
    {
        case settings_state::SETTING:
            m_item->rangeIndex = (m_item->rangeIndex + 1) % m_item->rangeSize;
            m_item->value = m_item->range[m_item->rangeIndex];
            break;
        case settings_state::CONFIG:
            break;
        case settings_state::MOISTURE:
            return menu_state::CALIBRATION;
        default:
            break;
    }
    return menu_state::NO_CHANGE;
}

menu_state MENU_SETTINGS::buttonBR()
{
    return menu_state::MENU_MAIN;
}
