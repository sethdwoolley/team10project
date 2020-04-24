/**
 * @file rgb.h
 * @author Oliver Newman (oliver@olivernewman.co.uk)
 * @brief A class for controlling rgb LEDs
 * @version 0.0.1
 * @date 2020-03-23
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef RGB_H
#define RGB_H

/// define value to turn LED off
#define OFF     0,0,0
/// define value to turn LED red
#define RED     255,0,0
/// define value to turn LED green
#define GREEN   0,255,0
/// define value to turn LED blue
#define BLUE    0,0,255

/// define value to flash LED fast
#define FAST    100
/// define value to flash LED slowly
#define SLOW    1000

/**
 * @brief A class for controlling rgb LEDs
 */
class RGB
{
private:
    int m_rPin, m_gPin, m_bPin;
    int m_rVal, m_gVal, m_bVal;

    void setValues(int r, int g, int b);
    
public:
    RGB(int rPin, int gPin, int bPin);
    void setColour(int r, int g, int b);
    void getValues(int *rgbVals);
    void flashLED(int r, int g, int b, int speed, 
                    int nTimes, bool stayOn = false);
    void init();
};

#endif // RGB_H