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
    Window *getActiveWindow(void);
    int getNumWindows(void);

    void setActiveWindow(Window *newActiveWindow);
    void repaintActive(void);

private:
    std::vector<Window *> windows;
};
