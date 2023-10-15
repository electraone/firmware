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
 * @file luaTimer.h
 *
 * @brief Implements a Lua instance of a hardware timer
 */

#pragma once

#include "luaIntegrationBase.h"

int luaopen_timer(lua_State *L);

int timer_enable(lua_State *L);
int timer_disable(lua_State *L);
int timer_isEnabled(lua_State *L);
int timer_setPeriod(lua_State *L);
int timer_getPeriod(lua_State *L);
int timer_setBpm(lua_State *L);
int timer_getBpm(lua_State *L);
void timer_runCallback(lua_State *L);

static const luaL_Reg timer_functions[] = {
    { "enable", timer_enable },       { "disable", timer_disable },
    { "isEnabled", timer_isEnabled }, { "setPeriod", timer_setPeriod },
    { "getPeriod", timer_getPeriod }, { "setBpm", timer_setBpm },
    { "getBpm", timer_getBpm },       { NULL, NULL }
};
