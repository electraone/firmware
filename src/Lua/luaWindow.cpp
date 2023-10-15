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
 * @file luaWindow.cpp
 *
 * @brief An implementation of the Window object for E1 Lua. 
 */

#include "luaWindow.h"
#include "Button.h"
#include "System.h"

bool lua_windowRepaintEnabled;

int luaopen_window(lua_State *L)
{
    luaL_newlib(L, window_functions);
    return (1);
}

int window_addAndMakeVisible(lua_State *L)
{
    lua_settop(L, 1);

    Button *component =
        *reinterpret_cast<Button **>(luaL_checkudata(L, 1, "Component"));

    if (component) {
        System::windowManager.getActiveWindow()->addAndMakeVisible(component);
        System::windowManager.repaintActive();
    }

    return (0);
}

int window_repaint([[maybe_unused]] lua_State *L)
{
    System::windowManager.repaintActive();
    return (0);
}

int window_clear([[maybe_unused]] lua_State *L)
{
    return (0);
}

int window_stop([[maybe_unused]] lua_State *L)
{
    System::tasks.disableRepaintGraphics();
    System::tasks.clearRepaintGraphics();
    return (0);
}

int window_resume([[maybe_unused]] lua_State *L)
{
    System::tasks.clearRepaintGraphics();
    System::tasks.enableRepaintGraphics();
    System::windowManager.repaintActive();
    return (0);
}

int window_findChildById(lua_State *L)
{
    lua_settop(L, 1);
    int id = luaL_checkinteger(L, -1);

    Button *component = reinterpret_cast<Button *>(
        System::windowManager.getActiveWindow()->findChildById(id));

    if (component) {
        *reinterpret_cast<Button **>(lua_newuserdata(L, sizeof(Button *))) =
            component;

        luaL_getmetatable(L, "Component");
        lua_setmetatable(L, -2);

        return (1);
    }

    return (0);
}
