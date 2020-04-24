#ifndef SCREEN_H
#define SCREEN_H

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class SCREEN
{
private:
    Adafruit_SSD1306 m_display;
public:
    SCREEN(int screenW = 128, int screenH = 32, TwoWire *wire = &Wire, int resetPin = -1);
    bool init();

    Adafruit_SSD1306 *getDisplay();

    int getXPositionFromLeft(int column ,int textSize);
    int getXPositionFromRight(String str, int textSize);
    int getYPositionFromTop(int row, int textSize);
    int getYPositionFromBottom(int textSize);
    int centerText(String str, int textSize);

    void drawControls(bool topRight = true, bool bottomRight = true, bool topLeft = true, bool bottomLeft = true);
};

#endif // DISPLAY_H