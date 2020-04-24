/**
 * @file menu_main.h
 * @author Oliver Newman (oliver@olivernewman.co.uk)
 * @brief subclass for the main menu
 * @version 0.0.1
 * @date 2020-04-05
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef MENU_MAIN_H
#define MENU_MAIN_H

#include "menu.h"

#include <Arduino.h>

#include "../screen/screen.h"

class MENU_MAIN : public MENU
{
private:
    const char m_items[3][16];
    int m_topOption;
public:
    MENU_MAIN();
    virtual void        draw(SCREEN *screen);
    virtual void        reset();
    virtual menu_state  buttonTL();
    virtual menu_state  buttonBL();
    virtual menu_state  buttonTR();
    virtual menu_state  buttonBR();
};

#endif // MENU_MAIN_H