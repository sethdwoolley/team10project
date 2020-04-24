/**
 * @file menu_calibration.h
 * @author Oliver Newman (oliver@olivernewman.co.uk)
 * @brief 
 * @version 0.0.1
 * @date 2020-04-10
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef CALIBRATION_H
#define CALIBRATION_H

#include "menu.h"
#include "menu_state.h"
#include "../screen/screen.h"
#include "../moisture/moisture.h"

// The calibration menus have to be controlled through the menu_state state 
// machine, as they require different things from the buttons.

class MENU_CALIBRATION : public MENU
{
private:
    calibration_state m_state;
    MOISTURE *m_moisture;

    menu_state buttonControls();

    calibration_state calibrateMoistureMinStart(SCREEN *screen);
    calibration_state calibrateMoistureMinRun(SCREEN *screen);
    calibration_state calibrateMoistureMaxStart(SCREEN *screen);
    calibration_state calibrateMoistureMaxRun(SCREEN *screen);
    void calibrateFinished(SCREEN *screen);
public:
    MENU_CALIBRATION(MOISTURE *moisture);
    virtual void draw(SCREEN *screen);
    virtual void reset();

    virtual menu_state buttonTL();
    virtual menu_state buttonBL();
    virtual menu_state buttonTR();
    virtual menu_state buttonBR();
};

#endif
