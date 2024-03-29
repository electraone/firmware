/*
* Electra One MIDI Controller Firmware
* See COPYRIGHT file at the top of the source tree.
*
* This product includes software developed by the
* Electra One Project (http://electra.one/).
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.
*/

/**
 * @file WindowManager.cpp
 *
 * @brief An implemantion of Window keeping manager.
 */

#include "WindowManager.h"
#include "ButtonBroadcaster.h"
#include "System.h"

WindowManager::WindowManager() : activeIndex(0)
{
}

void WindowManager::addWindow(Window *windowToAdd)
{
    windows.push_back(windowToAdd);
    windowToAdd->setParentWindow(getActiveWindow());
    windowToAdd->assignAllButtons();
    setActiveWindow(windowToAdd);
}

void WindowManager::removeWindow(Window *windowToRemove)
{
    uint8_t index = 0;
    for (auto i = windows.begin(); i != windows.end(); i++) {
        if (*i == windowToRemove) {
            windows.erase(i--);
            if (activeIndex == index) {
                if (activeIndex > 0) {
                    setActiveIndex(activeIndex - 1);
                }
            }

            Window *parentWindow = windowToRemove->getParentWindow();
            if (parentWindow) {
                parentWindow->buttonBroadcaster.resumeListener(parentWindow);
                parentWindow->resetActiveTouch();
                parentWindow->resetAllActivePotComponents();
                parentWindow->repaint();
            }
            return;
        }
        index++;
    }
}

std::vector<Window *> &WindowManager::getWindows(void)
{
    return (windows);
}

uint8_t WindowManager::getNumWindows(void)
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
    if (activeIndex != index) {
        auto oldWindow = getWindow(activeIndex);
        auto newWindow = getWindow(index);

        if (oldWindow) {
            oldWindow->buttonBroadcaster.suspendListener(oldWindow);
            oldWindow->setActive(false);
        }

        if (newWindow) {
            newWindow->buttonBroadcaster.resumeListener(newWindow);
            newWindow->setActive(true);
        }

        activeIndex = index;
    }
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

/**
 * List all registered windows
 */
void WindowManager::repaintAll(void)
{
    uint8_t index = 0;

    for (const auto &window : windows) {
        System::logger.write(
            LOG_ERROR,
            "repainting window: index=%d, name=%s, address=%x, "
            "active=%d, visible=%d",
            index,
            window->getName(),
            window,
            index == activeIndex,
            window->isVisible());
        if (window->isVisible()) {
            window->repaint();
        }
        index++;
    }
}

/**
 * List all registered windows
 */
void WindowManager::listWindows(void)
{
    uint8_t index = 0;

    for (const auto &window : windows) {
        System::logger.write(
            LOG_ERROR,
            "listWindows: index=%d, name=%s, address=%x, active=%d",
            index,
            window->getName(),
            window,
            index == activeIndex);
        index++;
    }
}
