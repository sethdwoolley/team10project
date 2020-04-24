#include "menu_startup.h"

#include "menu_state.h"

#include "team_logo.h"

#include <Adafruit_SSD1306.h>

MENU_STARTUP::MENU_STARTUP() : MENU("Startup", 1), m_progress(0), m_max_progress(100), m_loadingText("")
{
    
}

void MENU_STARTUP::progress(int p)
{
    m_progress += p;
    if (m_progress > m_max_progress) m_progress = m_max_progress;
}

void MENU_STARTUP::setMaxProgress(int max)
{
    m_max_progress = max;
}

void MENU_STARTUP::setLoadText(const char *text)
{
    m_loadingText = text;
}

void MENU_STARTUP::drawLogo(SCREEN *screen)
{
    Adafruit_SSD1306 *display = screen->getDisplay();

    display->clearDisplay();

    display->drawBitmap(0,0,team_logo,128,32,SSD1306_WHITE);

    display->display();
}

void MENU_STARTUP::drawLoading(SCREEN *screen)
{
    Adafruit_SSD1306 *display = screen->getDisplay();
    String finished = "Complete!";

    int textSize = 1;
    int optionSize = 2;

    int progressBarFill = map(m_progress, 0, m_max_progress, 0, finished.length() * 6 * optionSize);

    display->clearDisplay();
    

    display->setTextSize(textSize);
    display->setTextColor(SSD1306_WHITE);

    display->setCursor(screen->centerText(m_loadingText, textSize),screen->getYPositionFromTop(0,textSize));
    display->println(m_loadingText);
    
    display->drawRect(screen->centerText(finished, optionSize), 
                    screen->getYPositionFromTop(2,textSize) + 2, 
                    finished.length() * 6 * optionSize, 12, SSD1306_WHITE);
    display->fillRect(screen->centerText(finished, optionSize), 
                    screen->getYPositionFromTop(2,textSize) + 2, 
                    progressBarFill, 12, SSD1306_WHITE);

    display->display();
}

void MENU_STARTUP::draw(SCREEN *screen)
{
    if (m_selected == 0)
    {
        drawLogo(screen);
        m_selected++;
        delay(2000);
    }
    
    drawLoading(screen);
}

void MENU_STARTUP::reset()
{

}

menu_state MENU_STARTUP::buttonTL()
{
    return menu_state::NO_CHANGE;
}

menu_state MENU_STARTUP::buttonBL()
{
    return menu_state::NO_CHANGE;
}

menu_state MENU_STARTUP::buttonTR()
{
    return menu_state::NO_CHANGE;
}

menu_state MENU_STARTUP::buttonBR()
{
    return menu_state::NO_CHANGE;
}
