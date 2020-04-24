#ifndef TELEMETRY_H
#define TELEMETRY_H

class TELEMETRY
{
protected:
    int         m_pin;
    const char *m_name;
    const char *m_units;
    int         m_decPlaces;
    float       m_lastReading;

    TELEMETRY(int pin, const char *name, const char *units, int decPlaces) 
        : m_pin(pin), m_name(name), m_units(units), m_decPlaces(decPlaces) {}
public:
    virtual void        init() = 0;
    
    const char *getName();
    const char *getUnits();
    int         getDecPlaces();
    float       getLastReading();
};

#endif // TELEMETRY_H