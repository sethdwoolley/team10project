/**
 * @file menu_main.cpp
 * @author Oliver Newman (oliver@olivernewman.co.uk)
 * @brief subclass for the main menu
 * @version 0.0.1
 * @date 2020-04-05
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "menu_main.h"

#include "menu.h"
#include "menu_state.h"
#include "../screen/screen.h"

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

MENU_MAIN::MENU_MAIN() : MENU("Main Menu", 3), m_items({"Telemetry", "Settings", "About"}), m_topOption(0)
{
    
}

void MENU_MAIN::draw(SCREEN *screen)
{
    Adafruit_SSD1306 *display = screen->getDisplay();

    display->clearDisplay();

    screen->drawControls(true, false);

    int textSize = 1;
    int optionSize = 2;

    display->setTextSize(textSize);
    
    // Scrollbar if there is more than 3 menu items
    if (m_menuSize > 3)
    {
        display->drawLine(screen->getXPositionFromRight(" ",2) -6, 32.0 / m_menuSize * m_topOption, 
                        screen->getXPositionFromRight(" ",2) -6, (4 + m_topOption) * 32.0 / m_menuSize, 
                        SSD1306_WHITE);
        display->drawLine(screen->getXPositionFromRight(" ",2) -7, 32.0 / m_menuSize * m_topOption, 
                        screen->getXPositionFromRight(" ",2) -7, (4 + m_topOption) * 32.0 / m_menuSize, 
                        SSD1306_WHITE);
    }

    display->setCursor(screen->centerText(m_title, textSize),screen->getYPositionFromTop(0,textSize));
    display->println(m_title);

    for (int i = m_topOption, j = 1; i < m_topOption + (m_menuSize > 3 ? 3 : m_menuSize); i++, j++)
    {
        if (m_selected == i)
            display->setTextColor(SSD1306_BLACK, SSD1306_WHITE);
        else 
            display->setTextColor(SSD1306_WHITE, SSD1306_BLACK);
        display->setCursor(15,screen->getYPositionFromTop(j,textSize));
        display->println(String(m_items[i]));
    }

    display->display();
}

void MENU_MAIN::reset()
{
    m_selected = 0;
    m_topOption = 0;
}

menu_state MENU_MAIN::buttonTL()
{
    m_selected = m_selected > 0 ? m_selected -1 : m_menuSize -1;
    if (m_selected < m_topOption) m_topOption = m_selected;
    return menu_state::NO_CHANGE;
}

menu_state MENU_MAIN::buttonBL()
{
    m_selected = ++m_selected % m_menuSize;
    if (m_selected > 2) m_topOption = m_selected - 2;
    else m_topOption = 0;
    return menu_state::NO_CHANGE;
}

menu_state MENU_MAIN::buttonTR()
{
    switch(m_selected)
    {
        case 0:
            return menu_state::MENU_TELEMETRY;
        case 1:
            return menu_state::MENU_SETTINGS;
        case 2:
            return menu_state::MENU_ABOUT;
        default:
            return menu_state::NO_CHANGE;
    }
}

menu_state MENU_MAIN::buttonBR()
{
    return menu_state::NO_CHANGE;
}