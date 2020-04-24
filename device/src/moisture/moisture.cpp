#include "moisture.h"
#include <Arduino.h>

/**
 * @brief Construct a new MOISTURE object
 * 
 * @param pin GPIO pin the sensor is attached to.
 */
MOISTURE::MOISTURE(int pin, Moisture_range range) : TELEMETRY(pin, "Soil Moisture", "%RH", 1), m_range(range)
{

}

/** 
 * @brief Attaches the sensor to the GPIO pin.
 */
void MOISTURE::init()
{
    pinMode(m_pin, INPUT);
}

 /** 
  * @brief Gets the relative moisture of the soil as a percentage.
  * @return a float as (%RH). The sensor has the peculiar property that the 
  * higher of the raw values is actually the minimum moisture level. To map 
  * this easily, we use the Arduino map() library function, which will handle 
  * the inversion of the range easily. We map to a range of 0-1000 and divide 
  * by 10 to maintain precision.
  * 
  * https://www.arduino.cc/reference/en/language/functions/math/map/
  */
float MOISTURE::getReading()
{
    m_lastReading = map(analogRead(m_pin), m_range.min, m_range.max, 0, 1000) / 10.0;
    return m_lastReading;
}

void MOISTURE::setMin(int min)
{
    m_range.min = min;
}

void MOISTURE::setMax(int max)
{
    m_range.max = max;
}

int MOISTURE::getRawReading()
{
    return analogRead(m_pin);
}