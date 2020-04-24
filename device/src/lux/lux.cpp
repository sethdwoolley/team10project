/**
 * @file lux.cpp
 * @author Oliver Newman (oliver@olivernewman.co.uk)
 * @brief Class for controlling a [TEMT6000](https://wiki.keyestudio.com/Ks0098_keyestudio_TEMT6000_Ambient_Light_Sensor) 
 * ambient light sensor.
 * @version 0.0.1
 * @date 2020-03-24
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include "lux.h"
#include "../telemetry/telemetry.h"

#include <Arduino.h>

/**
 * @brief Construct a new LUX object
 * 
 * @param pin GPIO pin for sensor
 */
LUX::LUX(int pin) : TELEMETRY(pin, "Light levels", "lx", 0)
{

}

/** 
 * @brief Attaches the sensor to the GPIO pin.
 */
void LUX::init()
{
    pinMode(m_pin, INPUT);
}

/**
 * @brief Get a reading from the sensor.
 * 
 * We convert the raw reading (0-1023) to lux using 
 * 
 * \f[lx = \frac{1000\alpha}{1024}\f].
 * 
 * We arrive at this as follows:
 * 
 * The sensor takes a 5V input, so we convert our reading into the range 0-5:
 * 
 * \f[V = \frac{5\alpha}{1024}\f]
 * 
 * To convert to current, \f$I = \frac{V}{R}\f$, and the sensor has a 10k 
 * resistor attached: 
 * 
 * \f[
 * I = \frac{V}{10000} = 
 * \frac{\frac{5\alpha}{1024}}{10000} = 
 * \frac{\alpha}{2048000}
 * \f]
 * 
 * From the [datasheet](https://www.vishay.com/docs/81579/temt6000.pdf), when 
 * 20lx is shined on the sensor, we get a current of 10\f$\mu\f$A. Similarly, 
 * when 100lx is shined onto the sensor, we get a typical current of 
 * 50\f$\mu\f$A. We therefore need to multiply by 1,000,000 to convert A to 
 * \f$\mu\f$A:
 * 
 * \f[
 * \mu A = 1000000\cdot\frac{\alpha}{2048000} = \frac{1000\alpha}{2048}
 * \f]
 * 
 * And multiply by 2 to get lx:
 * 
 * \f[
 * lx = 2\cdot\frac{1000\alpha}{2048} = \frac{1000\alpha}{1024}
 * \f]
 * 
 * @return int 
 */
float LUX::getReading()
{
    m_lastReading = (analogRead(m_pin) * 1000.0 / 1024.0);
    return m_lastReading;
}