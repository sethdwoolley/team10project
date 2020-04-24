/**
 * @file rgb.cpp
 * @author Oliver Newman (oliver@olivernewman.co.uk)
 * @brief Class to control an rgb LED
 * @version 0.0.1
 * @date 2020-03-23
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include "rgb.h"
#include <Arduino.h>

/**
 * @brief Construct a new RGB object
 * 
 * @param rPin GPIO pin for the red LED
 * @param gPin GPIO pin for the green LED
 * @param bPin GPIO pin for the blue LED
 */
RGB::RGB(int rPin, int gPin, int bPin) 
    : m_rPin(rPin), m_gPin(gPin), m_bPin(bPin),
        m_rVal(255), m_gVal(255), m_bVal(155)
{

}

/**
 * @brief Store the current rgb values
 * 
 * @param r current red value (0-255)
 * @param g current green value (0-255)
 * @param b current blue value (0-255)
 */
void RGB::setValues(int r, int g, int b)
{
    m_rVal = r;
    m_gVal = g;
    m_bVal = b;
}

/**
 * @brief Retrieve the current rgb values and store them in the given array
 * 
 * @param rgbVals array in which to store the values
 */
void RGB::getValues(int *rgbVals)
{
    rgbVals[0] = m_rVal;
    rgbVals[1] = m_gVal;
    rgbVals[2] = m_bVal;
}

/**
 * @brief Set the LED to the given colour and store the given values
 * 
 * NOTE: The macros OFF, RED, BLUE, GREEN have been defined as default values
 * for the r,g,b params (e.g.\ OFF is defined as 0,0,0)
 * 
 * @param r red value (0-255)
 * @param g green value (0-255)
 * @param b blue value (0-255)
 */
void RGB::setColour(int r, int g, int b)
{
    /* 
     * The LED is common anode, meaning that it is passing current to the rgb
     * pins. Therefore, the brighter we want the LED, the less current we want
     * to pass to those pins. We can subtract the current value from 255 to 
     * invert the values as required.
     */
    analogWrite(m_rPin, 255-r);
    analogWrite(m_gPin, 255-g);
    analogWrite(m_bPin, 255-b);
    setValues(r,g,b);
}

/**
 * @brief Causes the LED to flash on and off the specified number of times.
 * 
 * NOTE: The macros OFF, RED, BLUE, GREEN have been defined as default values
 * for the r,g,b params (e.g.\ OFF is defined as 0,0,0)
 * 
 * @param r red value
 * @param g green value
 * @param b blue value
 * @param speed how quickly (in milliseconds) the led should flash.
 * @param nTimes how many times the LED should flash. NOTE: if the LED is 
 * already on and set to the same colour, the first flash will be missed.
 * @param stayOn should the LED stay on after flashing? NOTE: the macros FAST 
 * and SLOW have been defined as default values.
 */
void RGB::flashLED(int r, int g, int b, int speed, int nTimes, bool stayOn)
{
    speed /= 2;
    for (int i = 0; i < nTimes; i++)
    {
        setColour(r,g,b);
        delay(speed);
        setColour(OFF);
        delay(speed);
    }
    if (stayOn) setColour(r,g,b);
}

/**
 * @brief Attaches the GPIO pins to the LED
 * 
 */
void RGB::init()
{
    pinMode(m_rPin, OUTPUT);
    pinMode(m_gPin, OUTPUT);
    pinMode(m_bPin, OUTPUT);
    setColour(OFF);
}