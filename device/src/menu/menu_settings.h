#ifndef MENU_SETTINGS_H
#define MENU_SETTINGS_H

#include "menu.h"

#include <Arduino.h>

#include "../screen/screen.h"
#include "menu_state.h"
#include "../settings.h"

#include "../moisture/moisture.h"

class MENU_SETTINGS : public MENU
{
private:
    Settings *m_settings;
    Settings_Item *m_item;
    settings_state m_state;

    MOISTURE *m_moisture;

    void drawDefault(SCREEN *screen);
    void drawConfig(SCREEN *screen);
    void drawMoisture(SCREEN *screen);    
    void setState();
public:
    MENU_SETTINGS(Settings *settings);
    virtual void        draw(SCREEN *screen);
    virtual void        reset();
    virtual menu_state  buttonTL();
    virtual menu_state  buttonBL();
    virtual menu_state  buttonTR();
    virtual menu_state  buttonBR();
};

#endif // MENU_SETTINGS_H