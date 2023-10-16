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
 * @file System.h
 *
 * @brief A representation of the system layer running on the
 *  Hardware.
 */

#pragma once

#include "Hardware.h"
#include "SystemTasks.h"
#include "WindowManager.h"
#include "RepaintManager.h"
#include "Context.h"
#include "RuntimeInfo.h"
#include "Logger.h"

extern "C" {
#include "lauxlib.h"
#include "lua.h"
#include "lualib.h"
}

class System
{
public:
    System() = delete;

    static void initialise(void);

    static SystemTasks tasks;
    static WindowManager windowManager;
    static RepaintManager repaintManager;
    static Context context;
    static RuntimeInfo runtimeInfo;
    static Logger logger;
    static lua_State *L;

    static bool repaintActive;
    static bool sysExBusy;
};
