#include "menu_about.h"

#include "menu_state.h"

#include "team_logo.h"
#include "uol_logo.h"

#include <Adafruit_SSD1306.h>

MENU_ABOUT::MENU_ABOUT() : MENU("About", 1)
{
    
}

void MENU_ABOUT::drawLogo(SCREEN *screen)
{
    Adafruit_SSD1306 *display = screen->getDisplay();

    display->clearDisplay();

    if (m_selected == 0)
        display->drawBitmap(0,0,team_logo,128,32,SSD1306_WHITE);
    else
        display->drawBitmap(0,0,uol_logo,128,32,SSD1306_WHITE);

    display->display();
}

void MENU_ABOUT::drawTeamNames(SCREEN *screen)
{
    Adafruit_SSD1306 *display = screen->getDisplay();
    const char *header = "Team 10";
    const char *team[] = { "Teo Marginean", "Oliver Newman", "Damian Poclitar","Luke Shore-Jones", "Owen Williamson", "Seth Woolley" };
    int startingIndex = m_selected == 1 ? 0 : 3;
    
    display->clearDisplay();

    int textSize = 1;
    int optionSize = 2;

    display->setTextSize(textSize);
    display->setTextColor(SSD1306_WHITE);

    display->setCursor(screen->centerText(m_title, textSize),screen->getYPositionFromTop(0,textSize));
    display->println(m_title);
    
    for (int i = startingIndex, j = 1; i < startingIndex + 3; i++, j++)
    {
        display->setCursor(0,screen->getYPositionFromTop(j,textSize));
        display->println(team[i]);
    }

    display->display();
}

void MENU_ABOUT::draw(SCREEN *screen)
{
    switch (m_selected)
    {
        case 0:
        case 3:
            drawLogo(screen);
            break;
        case 1:
        case 2:
            drawTeamNames(screen);
            break;
        default:
            break;
    }
}

void MENU_ABOUT::reset()
{
    m_selected = 0;
}

menu_state MENU_ABOUT::buttonPress()
{
    switch(m_selected)
    {
        case 0:
        case 1:
        case 2:
            m_selected++;
            return menu_state::NO_CHANGE;
        default:
            m_selected = 0;
            return menu_state::MENU_MAIN;
    }
}

menu_state MENU_ABOUT::buttonTL()
{
    return buttonPress();
}

menu_state MENU_ABOUT::buttonBL()
{
    return buttonPress();
}

menu_state MENU_ABOUT::buttonTR()
{
    return buttonPress();
}

menu_state MENU_ABOUT::buttonBR()
{
    return buttonPress();
}