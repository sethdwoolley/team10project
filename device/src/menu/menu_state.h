#ifndef MENU_STATE_H
#define MENU_STATE_H

enum class menu_state
{
    NO_CHANGE,

    MENU_STARTUP,
    MENU_OFF,
    MENU_MAIN,
    MENU_TELEMETRY,
    MENU_SETTINGS,
    MENU_ABOUT,
    CALIBRATION,
    CONFIG_SERVER
};

enum class settings_state
{
    SETTING,
    CONFIG,
    MOISTURE
};

enum class calibration_state
{
    NO_CHANGE,
    
    MIN_START,
    MIN_RUN,
    MAX_START,
    MAX_RUN,

    FINISHED
};

#endif // MENU_STATE_H