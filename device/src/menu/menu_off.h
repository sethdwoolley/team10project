#ifndef MENU_OFF_H
#define MENU_OFF_H

#include "menu.h"

#include <Arduino.h>

#include "../screen/screen.h"

class MENU_OFF : public MENU
{
public:
    MENU_OFF();
    virtual void        draw(SCREEN *screen);
    virtual void        reset();
    virtual menu_state  buttonTL();
    virtual menu_state  buttonBL();
    virtual menu_state  buttonTR();
    virtual menu_state  buttonBR();
};

#endif // MENU_MAIN_H