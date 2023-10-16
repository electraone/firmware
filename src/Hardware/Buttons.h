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
 * @file Buttons.h
 *
 * @brief A collection of all hardware Buttons.
 */

#pragma once

#include "settings.h"
#include "HardwareButton.h"

// \todo replace macros with enum
#define BUTTON_LEFT_TOP 0
#define BUTTON_LEFT_MIDDLE 1
#define BUTTON_LEFT_BOTTOM 2
#define BUTTON_RIGHT_TOP 3
#define BUTTON_RIGHT_MIDDLE 4
#define BUTTON_RIGHT_BOTTOM 5

/**
 * Buttons class
 *
 * The class holds an array of hardware buttons. Their ids and their position
 * in the private buttons[] array matters and must reflect hardware layout
 */
class Buttons
{
public:
    HardwareButton *operator[](const uint8_t id);
    HardwareButton *getNext(void);

private:
    HardwareButton buttons[NR_OF_HW_BUTTONS] = {
        HardwareButton(BUTTON_LEFT_TOP),     HardwareButton(BUTTON_LEFT_MIDDLE),
        HardwareButton(BUTTON_LEFT_BOTTOM),  HardwareButton(BUTTON_RIGHT_TOP),
        HardwareButton(BUTTON_RIGHT_MIDDLE), HardwareButton(BUTTON_RIGHT_BOTTOM)
    };
    uint8_t currentId = 0;
};
