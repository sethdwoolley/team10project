#ifndef MENU_TELEMETRY_H
#define MENU_TELEMETRY_H

#include "menu.h"

#include <Arduino.h>

#include "../screen/screen.h"
#include "../telemetry/telemetry.h"
#include "../temperature/temperature.h"
#include "../humidity/humidity.h"
#include "../lux/lux.h"
#include "../moisture/moisture.h"

class MENU_TELEMETRY : public MENU
{
private:
    TELEMETRY   *m_telemetry;
    TEMPERATURE *m_temp;
    HUMIDITY    *m_humidity;
    LUX         *m_lux;
    MOISTURE    *m_moisture;

    void setTelemetry();
public:
    MENU_TELEMETRY(TEMPERATURE *t, HUMIDITY *h, LUX *l, MOISTURE *m);
    virtual void        draw(SCREEN *screen);
    virtual void        reset();
    virtual menu_state  buttonTL();
    virtual menu_state  buttonBL();
    virtual menu_state  buttonTR();
    virtual menu_state  buttonBR();
};

#endif // MENU_TELEMETRY_H