#include "humidity.h"

#include "../telemetry/telemetry.h"

HUMIDITY::HUMIDITY(DHT *dht) : TELEMETRY(-1, "Humidity", "%", 1), m_dht(dht)
{

}

void HUMIDITY::init()
{
}

float HUMIDITY::getReading()
{
    m_lastReading = m_dht->readHumidity();
    return m_lastReading;
}