/**
 * @file moisture.h
 * @author Oliver Newman (oliver@olivernewman.co.uk)
 * @brief A class for controlling the [SEN0193](https://wiki.dfrobot.com/Capacitive_Soil_Moisture_Sensor_SKU_SEN0193) 
 * capacitive soil moisture sensor from DFROBOT
 * @version 0.0.2
 * @date 2020-04-05
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef MOISTURE_H
#define MOISTURE_H

#include "../telemetry/telemetry.h"

typedef struct
{
    bool valid;
    int min;
    int max;
} Moisture_range;

/**
 * @brief A class for controlling the [SEN0193](https://wiki.dfrobot.com/Capacitive_Soil_Moisture_Sensor_SKU_SEN0193) 
 * capacitive soil moisture sensor from DFROBOT
 */
class MOISTURE : public TELEMETRY
{
private:
    Moisture_range m_range;
public:
    MOISTURE(int pin, Moisture_range range);
    virtual void init();
    virtual float getReading();

    void setMin(int min);
    void setMax(int max);
    
    int getRawReading();
    
};

#endif // MOISTURE_H