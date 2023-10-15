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
 * @file luaSysexBlock.h
 *
 * @brief An implementation of the SysexBlock object for E1 Lua. 
 */

#pragma once

#include "App.h"

extern "C" {
#include "lauxlib.h"
#include "lua.h"
#include "lualib.h"
}

int sysexBlock_create(lua_State *L);
int sysexBlock_delete(lua_State *L);
int sysexBlock_close(lua_State *L);
int sysexBlock_write(lua_State *L);
int sysexBlock_read(lua_State *L);
int sysexBlock_seek(lua_State *L);
int sysexBlock_peek(lua_State *L);
int sysexBlock_getLength(lua_State *L);
int sysexBlock_getManufacturerSysexId(lua_State *L);

void sysexBlock_register(lua_State *L);

static const luaL_Reg sysexBlock_functions[] = {
    { "close", sysexBlock_close },
    { "write", sysexBlock_write },
    { "read", sysexBlock_read },
    { "seek", sysexBlock_seek },
    { "peek", sysexBlock_peek },
    { "getLength", sysexBlock_getLength },
    { "getManufacturerSysexId", sysexBlock_getManufacturerSysexId },
    { NULL, NULL }
};
