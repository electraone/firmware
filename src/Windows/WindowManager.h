#pragma once

#include "Window.h"
#include <vector>

class WindowManager
{
public:
    WindowManager();
    virtual ~WindowManager() = default;

    bool addWindow(Window *windowToAdd);
    void removeWindow(Window *windowToRemove);

    Window *getWindow(uint8_t index);
    int getNumWindows(void);

    Window *getActiveWindow(void);
    void setActiveWindow(Window *newActiveWindow);

    void    setActiveIdx(uint8_t);
    uint8_t getActiveIdx();

    void repaintActive(void);

private:
    void activate(uint8_t idx);

    std::vector<Window *> windows;
    uint8_t activeIdx = 0;
};
