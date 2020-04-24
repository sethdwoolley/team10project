#ifndef HUMIDITY_H
#define HUMIDITY_H

#include "../telemetry/telemetry.h"

#include <DHT.h>

class HUMIDITY : public TELEMETRY
{
private:
    DHT *m_dht;
public:
    HUMIDITY(DHT *dht);
    virtual void init();
    virtual float getReading();
};

#endif // HUMIDITY_H