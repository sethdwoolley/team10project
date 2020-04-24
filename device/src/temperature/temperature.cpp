#include "temperature.h"

#include "../telemetry/telemetry.h"

TEMPERATURE::TEMPERATURE(DHT *dht) : TELEMETRY(-1, "Temperature", "C", 1), m_dht(dht)
{

}

void TEMPERATURE::init()
{
}

float TEMPERATURE::getReading()
{
    m_lastReading = m_dht->readTemperature();
    return m_lastReading;
}