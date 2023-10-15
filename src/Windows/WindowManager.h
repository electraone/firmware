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
 * @file WindowManager.h
 *
 * @brief An implemantion of Window keeping manager.
 */

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

    std::vector<Window *> &getWindows(void);
    uint8_t getNumWindows(void);
    Window *getWindow(uint8_t index);

    Window *getActiveWindow(void);
    void setActiveWindow(Window *newActiveWindow);

    void setActiveIndex(uint8_t);
    uint8_t getActiveIndex(void);

    void repaintActive(void);
    void repaintAll(void);

    void listWindows(void);

private:
    void activate(uint8_t index);

    std::vector<Window *> windows;
    uint8_t activeIndex;
};
