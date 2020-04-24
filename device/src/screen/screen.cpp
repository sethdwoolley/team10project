#include "screen.h"

SCREEN::SCREEN(int screenW, int screenH, TwoWire *wire, int resetPin) 
    : m_display(screenW, screenH, wire, resetPin)
{
    
}

bool SCREEN::init()
{
    if (!m_display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
        Serial.println(F("SSD1306 allocation failed"));
        return false;
    }
        
    m_display.clearDisplay();
    m_display.display();

    return true;
}

Adafruit_SSD1306 *SCREEN::getDisplay()
{
    return &m_display;
}

int SCREEN::getXPositionFromLeft(int column ,int textSize)
{
    return column * textSize * 6;
}

int SCREEN::getYPositionFromTop(int row, int textSize)
{
    // VALIDATION NEEDED
    // Screen is 32px high, and each character is (8px * textsize) tall
    return row * 8 * textSize;
}

int SCREEN::getXPositionFromRight(String str, int textSize)
{
    // VALIDATION NEEDED
    // Screen is 128px wide, and each character is (6px * textSize) wide
    return 128 - (6 * str.length() * textSize);
}

int SCREEN::getYPositionFromBottom(int textSize)
{
    return 32 - (8 * textSize);
}

int SCREEN::centerText(String str, int textSize)
{
    return (128 - (str.length() * textSize * 6)) / 2;
}

void SCREEN::drawControls(bool topRight, bool bottomRight, bool topLeft, bool bottomLeft)
{
    int size = 2;

    char upArrow = (char)24;
    char downArrow = (char)25;
    char rightArrow = (char)26;
    char leftArrow = (char)27;

    m_display.setTextSize(size);
    m_display.setTextColor(SSD1306_WHITE);

    if (topLeft)
    {
        m_display.setCursor(0,getYPositionFromTop(0,size));
        m_display.println(upArrow);
    }

    if (bottomLeft)
    {
        m_display.setCursor(0,getYPositionFromBottom(size));
        m_display.println(downArrow);
    }
    
    if (topRight)
    {
        m_display.setCursor(getXPositionFromRight(String(rightArrow),size),0);
        m_display.println(rightArrow);
    }

    if (bottomRight)
    {
        m_display.setCursor(getXPositionFromRight(String(leftArrow),size),
                        getYPositionFromBottom(size));
        m_display.println(leftArrow);
    }

    m_display.drawLine(getXPositionFromLeft(1,size), 0, getXPositionFromLeft(1,size), 32, SSD1306_WHITE);
    m_display.drawLine(getXPositionFromRight(String(rightArrow),size) -3, 0, 
                        getXPositionFromRight(String(rightArrow),size) -3, 32, 
                        SSD1306_WHITE);
}