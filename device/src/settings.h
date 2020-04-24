#ifndef SETTINGS_H
#define SETTINGS_H

typedef struct
{
    const char *name;
    /* Whilst it is slightly redundant to have a set value stored alongside a 
     * range of values and the current index, this allows users to set precise 
     * options from the configuration page as well as being able to select a
     * value from the range on the device itself.
     */
    int value;
    const char *unit;
    int rangeIndex;
    int range[20];
    int rangeSize;
} Settings_Item;

typedef struct 
{
    // For confirming values are in flash memory.
    bool valid;
    Settings_Item buttonSensitivity;
    Settings_Item timeoutInterval;
    Settings_Item publishInterval;
} Settings;

#endif // SETTINGS_H