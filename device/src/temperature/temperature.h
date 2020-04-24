#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include "../telemetry/telemetry.h"

#include <DHT.h>

class TEMPERATURE : public TELEMETRY
{
private:
    DHT *m_dht;
public:
    TEMPERATURE(DHT *dht);
    virtual void init();
    virtual float getReading();
};

#endif // TEMPERATURE_H