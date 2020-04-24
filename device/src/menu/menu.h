/**
 * @file menu.h
 * @author Oliver Newman (oliver@olivernewman.co.uk)
 * @brief abstract class for menus
 * @version 0.0.1
 * @date 2020-04-05
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef MENU_H
#define MENU_H

#include "menu_state.h"
#include "../screen/screen.h"

class MENU
{
protected:
    const char *m_title;
    int m_selected;
    int m_menuSize;

    MENU(const char *title, int size) : m_title(title), m_selected(0), m_menuSize(size) {}
public:
    
    virtual void        draw(SCREEN *screen) = 0;
    virtual void        reset() = 0;

    virtual menu_state  buttonTL() = 0;
    virtual menu_state  buttonBL() = 0;
    virtual menu_state  buttonTR() = 0;
    virtual menu_state  buttonBR() = 0;
};

#endif // MENU_H