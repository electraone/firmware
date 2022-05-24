#include "WindowManager.h"

WindowManager::WindowManager() : activeIndex(0)
{
}

bool WindowManager::addWindow(Window *windowToAdd)
{
    windows.push_back(windowToAdd);
    return (true);
}

void WindowManager::removeWindow(Window *windowToRemove)
{
    uint8_t index = 0;
    for (auto i = windows.begin(); i != windows.end(); i++) {
        if (*i == windowToRemove) {
            windows.erase(i);
            if (activeIndex == index) {
                if (activeIndex > 0) {
                    setActiveIndex(activeIndex - 1);
                }
            }
            // note: iterator is invalid!
            return;
        }
    }
}

int WindowManager::getNumWindows(void)
{
    return (windows.size());
}

Window *WindowManager::getWindow(uint8_t index)
{
    if (index >= windows.size()) {
        return nullptr;
    }
    return (windows[index]);
}

Window *WindowManager::getActiveWindow(void)
{
    return getWindow(activeIndex);
}

void WindowManager::setActiveWindow(Window *newActiveWindow)
{
    uint8_t index = 0;
    for (const auto &i : windows) {
        if (i == newActiveWindow) {
            setActiveIndex(index);
            return;
        }
        index++;
    }
}

void WindowManager::setActiveIndex(uint8_t index)
{
    if (index < windows.size()) {
        activate(index);
    }
}

uint8_t WindowManager::getActiveIndex()
{
    return activeIndex;
}

void WindowManager::activate(uint8_t index)
{
    activeIndex = index;

    // todo?
    // do we need to deactivate windows, so that they dont start processing events?

    // auto oldW = getWindow(activeIdx);
    // auto newW = getWindow(idx);
    // if(oldW) {
    //     oldW->setActive(false);
    // }

    // if(newW) {
    //     newW->setActive(true);
    // }
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
