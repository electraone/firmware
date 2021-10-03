#include "WindowManager.h"


WindowManager::WindowManager() : activeWindow(nullptr)
{
}

bool WindowManager::addWindow(Window *const windowToAdd)
{
    windows.push_back(windowToAdd);

    return (true);
}

void WindowManager::removeWindow(Window *const windowToRemove)
{
    // \todo add implementation
}

int WindowManager::getNumWindows()
{
    return (windows.size());
}

Window *WindowManager::getWindow(const int index)
{
    return windows[index];
}

Window *WindowManager::getActiveWindow()
{
    return (activeWindow);
}

/**
 * Assigns the active window
 *
 */
void WindowManager::setActiveWindow(Window *newActiveWindow)
{
    activeWindow = newActiveWindow;
}

/**
 * Repaints currently active window
 *
 */
void WindowManager::repaintActive(void)
{
    if (getActiveWindow()) {
        getActiveWindow()->repaint();
    }
}
