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
 * @file System.cpp
 *
 * @brief A representation of the system layer running on the
 *  Hardware.
 */

#include "System.h"
#include "Random.h"
#include "helpers.h"

void System::initialise(void)
{
    // Initialize Random number generator
    randomSeed(analogRead(0));

    // initialize non-volatile storage if not set
    runtimeInfo.read();

    // initialize USB device.

    // First time configuration
    // This is to bring older Electras to known state of the USB device
    // configuration.
    if (runtimeInfo.getUsbDevStatus() != 1) {
        runtimeInfo.setUsbDevStatus(1);
        runtimeInfo.setKeyboardStatus(0);
        runtimeInfo.setMouseStatus(0);
        runtimeInfo.setKeymediaStatus(0);
    }

    // Configure the USB device interfaces
    uint8_t keyboardEnabled = 0;
    uint8_t mouseEnabled = 0;
    uint8_t keymediaEnabled = 0;

    if (runtimeInfo.getUsbDevStatus() == 1) {
        keyboardEnabled = runtimeInfo.getKeyboardStatus();
        mouseEnabled = runtimeInfo.getMouseStatus();
        keymediaEnabled = runtimeInfo.getKeymediaStatus();
    }

    usb_init(keyboardEnabled, mouseEnabled, keymediaEnabled);
    System::logger.write(LOG_INFO, "USB device ports: initialised");
}

SystemTasks System::tasks;
WindowManager System::windowManager;
RepaintManager System::repaintManager(Hardware::screen, System::windowManager);
Context System::context;
RuntimeInfo System::runtimeInfo;
Logger System::logger;
lua_State *System::L = nullptr;

bool System::repaintActive = false;
bool System::sysExBusy = false;
