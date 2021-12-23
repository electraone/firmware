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
    uint8_t idx = 0;
    for(auto i = windows.begin();i != windows.end();i++) {
        if(*i == windowToRemove) {
            windows.erase(i);
            if(activeIdx == idx) {
                if(activeIdx > 0) {
                    setActiveIdx(activeIdx - 1);
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
    if(index >= windows.size()) {
        return nullptr;
    }
    return (windows[index]);
}

Window *WindowManager::getActiveWindow(void)
{
    return getWindow(activeIdx);   
}


void WindowManager::setActiveWindow(Window *newActiveWindow) {
    uint8_t idx = 0;
    for(const auto& i : windows) {
        if(i == newActiveWindow) {
            setActiveIdx(idx);
            return;
        }
        idx++;
    }
}

void WindowManager::setActiveIdx(uint8_t index) {
    if(index < windows.size()) {
        activate(index);
    }
}

uint8_t WindowManager::getActiveIdx() {
    return activeIdx;

}



void WindowManager::activate(uint8_t idx) {
    activeIdx = idx;

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
