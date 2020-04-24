#ifndef MENU_STARTUP_H
#define MENU_STARTUP_H

#include "menu.h"

#include <Arduino.h>

#include "../screen/screen.h"

class MENU_STARTUP : public MENU
{
private:
    const char *m_loadingText;
    int m_progress, m_max_progress;
    void drawLogo(SCREEN *screen);
    void drawLoading(SCREEN *screen);
public:
    MENU_STARTUP();
    virtual void        draw(SCREEN *screen);
    virtual void        reset();
    virtual menu_state  buttonTL();
    virtual menu_state  buttonBL();
    virtual menu_state  buttonTR();
    virtual menu_state  buttonBR();

    void progress(int p = 1);
    void setMaxProgress(int max = 100);
    void setLoadText(const char *text);
};

#endif // MENU_STARTUP_H