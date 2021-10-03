#pragma once

#include "Window.h"

class WindowManager
{
public:
    WindowManager();
    virtual ~WindowManager() = default;

    bool addWindow(Window *const windowToAdd);
    void removeWindow(Window *const windowToRemove);
    int getNumWindows();
    Window *getWindow(const int index);
    Window *getActiveWindow();

    void setActiveWindow(Window *newActiveWindow);
    void repaintActive(void);

private:
    Window *activeWindow;
    std::vector<Window *> windows;
};
