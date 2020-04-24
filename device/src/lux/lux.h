/**
 * @file lux.h
 * @author Oliver Newman (oliver@olivernewman.co.uk)
 * @brief A class for controlling a [TEMT6000](https://wiki.keyestudio.com/Ks0098_keyestudio_TEMT6000_Ambient_Light_Sensor) 
 * ambient light sensor.
 * @version 0.0.1
 * @date 2020-03-24
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef LUX_H
#define LUX_H

#include "../telemetry/telemetry.h"

/**
 * @brief A class for controlling a [TEMT6000](https://wiki.keyestudio.com/Ks0098_keyestudio_TEMT6000_Ambient_Light_Sensor) 
 * ambient light sensor.
 */
class LUX : public TELEMETRY
{
public:
    LUX(int pin);
    virtual void init();
    virtual float getReading();
};

#endif // LUX_H