#ifndef MENU_ABOUT_H
#define MENU_ABOUT_H

#include "menu.h"

#include <Arduino.h>

#include "../screen/screen.h"

class MENU_ABOUT : public MENU
{
private:
    void drawLogo(SCREEN *screen);
    void drawTeamNames(SCREEN *screen);
    menu_state buttonPress();
public:
    MENU_ABOUT();
    virtual void        draw(SCREEN *screen);
    virtual void        reset();
    virtual menu_state  buttonTL();
    virtual menu_state  buttonBL();
    virtual menu_state  buttonTR();
    virtual menu_state  buttonBR();
};

#endif // MENU_ABOUT_H