#pragma once

#include "Window.h"
#include <vector>

class WindowManager
{
public:
    WindowManager();
    virtual ~WindowManager() = default;

    void addWindow(Window *windowToAdd);
    void removeWindow(Window *windowToRemove);

    std::vector<Window *> getWindows(void);
    uint8_t getNumWindows(void);
    Window *getWindow(uint8_t index);

    Window *getActiveWindow(void);
    void setActiveWindow(Window *newActiveWindow);

    void setActiveIndex(uint8_t);
    uint8_t getActiveIndex(void);

    void repaintActive(void);

    void listWindows(void);

private:
    void activate(uint8_t index);

    std::vector<Window *> windows;
    uint8_t activeIndex;
};
