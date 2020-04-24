#include "menu_calibration.h"

#include "menu.h"
#include "menu_state.h"

#include "../screen/screen.h"

MENU_CALIBRATION::MENU_CALIBRATION(MOISTURE *moisture) : MENU("Calibration", -1), m_moisture(moisture)
{
    m_state = calibration_state::MIN_START;
}

void MENU_CALIBRATION::draw(SCREEN *screen)
{
    Serial.println();
    switch(m_state)
    {
        case calibration_state::MIN_START:
            Serial.println("MIN_START");
            calibrateMoistureMinStart(screen);
            break;
        case calibration_state::MIN_RUN:
            Serial.println("MIN_RUN");
            m_state = calibrateMoistureMinRun(screen);
            break;
        case calibration_state::MAX_START:
            Serial.println("MAX_START");
            calibrateMoistureMaxStart(screen);
            break;
        case calibration_state::MAX_RUN:
            Serial.println("MAX_RUN");
            m_state = calibrateMoistureMaxRun(screen);
            break;
        case calibration_state::FINISHED:
            Serial.println("FINISHED");
            calibrateFinished(screen);
            break;
        default:
            break;

    }
}

void MENU_CALIBRATION::reset()
{
    m_state = calibration_state::MIN_START;
}

calibration_state MENU_CALIBRATION::calibrateMoistureMinStart(SCREEN *screen)
{
    Adafruit_SSD1306 *display = screen->getDisplay();

    String line1 = "Hold the moisture";
    String line2 = "sensor in the air";
    String button = "START";

    int textSize = 1;
    int optionSize = 2;

    display->clearDisplay();

    display->setTextColor(SSD1306_WHITE);
    display->setTextSize(textSize);

    display->setCursor(screen->centerText(line1, textSize),screen->getYPositionFromTop(0,textSize));
    display->println(line1);

    display->setCursor(screen->centerText(line2, textSize),screen->getYPositionFromTop(1,textSize));
    display->println(line2);

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

    return calibration_state::NO_CHANGE;
}

calibration_state MENU_CALIBRATION::calibrateMoistureMinRun(SCREEN *screen)
{
    Adafruit_SSD1306 *display = screen->getDisplay();

    String line1 = "Calibrating minimum";
    String line2 = "moisture value";
    String finished = "Complete!";
    int textSize = 1;
    int optionSize = 2;

    int min = 0;
    int counter = 1000;
    int reading, progress;
    long current;

    display->clearDisplay();

    display->setTextColor(SSD1306_WHITE);
    display->setTextSize(textSize);

    display->setCursor(screen->centerText(line1, textSize),screen->getYPositionFromTop(0,textSize));
    display->println(line1);

    display->setCursor(screen->centerText(line2, textSize),screen->getYPositionFromTop(1,textSize));
    display->println(line2);

    current = millis();
    while (millis() - current < 5000)
    {
        progress = map(millis() - current, 0, 5000, 0, finished.length() * 6 * optionSize);

        display->drawRect(screen->centerText(finished, optionSize), 
                        screen->getYPositionFromTop(2,textSize) + 2, 
                        finished.length() * 6 * optionSize, 12, SSD1306_WHITE);
        display->fillRect(screen->centerText(finished, optionSize), 
                        screen->getYPositionFromTop(2,textSize) + 2, 
                        progress, 12, SSD1306_WHITE);
        display->display();
        reading = m_moisture->getRawReading();
        min = reading > min ? reading : min;
    }

    m_moisture->setMin(min);

    display->setTextColor(SSD1306_BLACK);
    display->setTextSize(textSize);

    display->setCursor(screen->centerText(finished, textSize),screen->getYPositionFromTop(3,textSize) - 4 );
    display->println(finished);
    display->display();

    delay (1000);
    
    return calibration_state::MAX_START;
}

calibration_state MENU_CALIBRATION::calibrateMoistureMaxStart(SCREEN *screen)
{
    Adafruit_SSD1306 *display = screen->getDisplay();

    String line1 = "Place the sensor";
    String line2 = "in water to line";
    String button = "START";

    int textSize = 1;
    int optionSize = 2;

    display->clearDisplay();

    display->setTextColor(SSD1306_WHITE);
    display->setTextSize(textSize);

    display->setCursor(screen->centerText(line1, textSize),screen->getYPositionFromTop(0,textSize));
    display->println(line1);

    display->setCursor(screen->centerText(line2, textSize),screen->getYPositionFromTop(1,textSize));
    display->println(line2);

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

    return calibration_state::NO_CHANGE;
}

calibration_state MENU_CALIBRATION::calibrateMoistureMaxRun(SCREEN *screen)
{
    Adafruit_SSD1306 *display = screen->getDisplay();

    String line1 = "Calibrating maximum";
    String line2 = "moisture value";
    String finished = "Complete!";
    int textSize = 1;
    int optionSize = 2;

    int max = 2000;
    int reading, progress;
    long current;

    display->clearDisplay();

    display->setTextColor(SSD1306_WHITE);
    display->setTextSize(textSize);

    display->setCursor(screen->centerText(line1, textSize),screen->getYPositionFromTop(0,textSize));
    display->println(line1);

    display->setCursor(screen->centerText(line2, textSize),screen->getYPositionFromTop(1,textSize));
    display->println(line2);

    current = millis();
    while (millis() - current < 5000)
    {
        progress = map(millis() - current, 0, 5000, 0, finished.length() * 6 * optionSize);

        display->drawRect(screen->centerText(finished, optionSize), 
                        screen->getYPositionFromTop(2,textSize) + 2, 
                        finished.length() * 6 * optionSize, 12, SSD1306_WHITE);
        display->fillRect(screen->centerText(finished, optionSize), 
                        screen->getYPositionFromTop(2,textSize) + 2, 
                        progress, 12, SSD1306_WHITE);
        display->display();
        reading = m_moisture->getRawReading();
        max = reading < max ? reading : max;
    }

    m_moisture->setMax(max);

    display->setTextColor(SSD1306_BLACK);
    display->setTextSize(textSize);

    display->setCursor(screen->centerText(finished, textSize),screen->getYPositionFromTop(3,textSize) - 4 );
    display->print(finished);
    display->display();

    delay (1000);
    return calibration_state::FINISHED;
}

void MENU_CALIBRATION::calibrateFinished(SCREEN *screen)
{
    Adafruit_SSD1306 *display = screen->getDisplay();

    int textSize = 1;
    int optionSize = 2;
    char rightArrow = (char)26;

    display->setCursor(display->getCursorX() + 2, display->getCursorY() - 3);
    display->setTextSize(2);
    display->println(rightArrow);
    display->display();
}

menu_state MENU_CALIBRATION::buttonControls()
{
    switch (m_state)
    {
        case calibration_state::MIN_START:
            m_state = calibration_state::MIN_RUN;
            break;
        case calibration_state::MAX_START:
            m_state = calibration_state::MAX_RUN;
            break;
        case calibration_state::FINISHED:
            return menu_state::MENU_SETTINGS;
        default:
            break;
    }

    return menu_state::NO_CHANGE;
}

menu_state MENU_CALIBRATION::buttonTL()
{
    return buttonControls();
}

menu_state MENU_CALIBRATION::buttonBL()
{
    return buttonControls();
}

menu_state MENU_CALIBRATION::buttonTR()
{
    return buttonControls();
}

menu_state MENU_CALIBRATION::buttonBR()
{
    return buttonControls();
}