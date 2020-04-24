#include "telemetry.h"

const char *TELEMETRY::getName()
{
    return m_name;
}

const char *TELEMETRY::getUnits()
{
    return m_units;
}

int TELEMETRY::getDecPlaces()
{
    return m_decPlaces;
}

float TELEMETRY::getLastReading()
{
    return m_lastReading;
}
