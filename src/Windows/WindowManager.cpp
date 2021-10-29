#include "WindowManager.h"

WindowManager::WindowManager()
{
}

bool WindowManager::addWindow(Window *windowToAdd)
{
    windows.push_back(windowToAdd);

    logMessage("xxxx Name: %s", windows[0]->getName());

    return (true);
}

void WindowManager::removeWindow(Window *windowToRemove)
{
    if (windows.back() == windowToRemove) {
        static_cast<void>(windows.pop_back());
    }
}

int WindowManager::getNumWindows(void)
{
    return (windows.size());
}

Window *WindowManager::getWindow(uint8_t index)
{
    return (windows[index]);
}

Window *WindowManager::getActiveWindow(void)
{
    if (windows.size() > 0) {
        return (windows.back());
    }

    return (nullptr);
}

/**
 * Repaints currently active window
 *
 */
void WindowManager::repaintActive(void)
{
    Window *w = getActiveWindow();

    if (w) {
        w->repaint();
    }
}
