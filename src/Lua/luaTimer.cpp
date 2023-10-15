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

#include "luaTimer.h"
#include "luaExtensionBase.h"

int luaopen_timer(lua_State *L)
{
    luaL_newlib(L, timer_functions);
    return 1;
}

int timer_enable(lua_State *L)
{
    lua_settop(L, 0);
    System::tasks.enableTimer();

    return (0);
}

int timer_disable(lua_State *L)
{
    lua_settop(L, 0);
    System::tasks.disableTimer();

    return (0);
}

int timer_isEnabled(lua_State *L)
{
    lua_pushboolean(L, System::tasks.isTimerEnabled());
    return (1);
}

int timer_setPeriod(lua_State *L)
{
    lua_settop(L, 1);

    int period = luaL_checkinteger(L, -1);

    if (period < 10) {
        return (luaL_error(L, "Minimum timer period is 10 mSec"));
    }

    System::tasks.timerSetInterval(period * 1000);

    return (0);
}

int timer_getPeriod(lua_State *L)
{
    unsigned long interval = System::tasks.timerGetInterval();

    lua_pushinteger(L, interval / 1000);
    return (1);
}

int timer_setBpm(lua_State *L)
{
    lua_settop(L, 1);

    int bmp = luaL_checkinteger(L, -1);

    if (bmp > 6000) {
        return (luaL_error(L, "Maximum timer BMP is 6000"));
    }

    System::tasks.timerSetInterval(60000000 / bmp);

    return (0);
}

int timer_getBpm(lua_State *L)
{
    unsigned long interval = System::tasks.timerGetInterval();

    lua_pushinteger(L, 60000000 / interval);
    return (1);
}

void timer_runCallback(lua_State *L)
{
    luaLE_getModuleFunction(L, "timer", "onTick");

    if (lua_isfunction(L, -1)) {
        if (lua_pcall(L, 0, 0, 0) != 0) {
            System::logger.write(LOG_LUA,
                                 "error running function 'timer.callback': %s",
                                 lua_tostring(L, -1));
        }
    }
}
