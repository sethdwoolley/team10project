#include "menu_off.h"

#include "menu_state.h"

MENU_OFF::MENU_OFF() : MENU("", 0)
{

}

void MENU_OFF::draw(SCREEN *screen)
{
    screen->getDisplay()->clearDisplay();
    screen->getDisplay()->display();
}

void MENU_OFF::reset()
{

}

menu_state MENU_OFF::buttonTL()
{
    return menu_state::MENU_MAIN;
}

menu_state MENU_OFF::buttonBL()
{
    return menu_state::MENU_MAIN;
}

menu_state MENU_OFF::buttonTR()
{
    return menu_state::MENU_MAIN;
}

menu_state MENU_OFF::buttonBR()
{
    return menu_state::MENU_MAIN;
}