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
 * @file luaIntegration.h
 *
 * @brief A collection of function to interface C++ and Lua
 * 
 * The module consists of generic non-E1 specific functionality.
 */

#pragma once

extern "C" {
#include "lauxlib.h"
#include "lua.h"
#include "lualib.h"
}

extern lua_State *L;

void luaLE_pushTableString(lua_State *L, const char *key, const char *value);
void luaLE_pushTableInteger(lua_State *L, const char *key, int value);
void luaLE_pushTableObject(lua_State *L, const char *key, const void *object);
void luaLE_pushArrayInteger(lua_State *L, int key, int value);
void luaLE_pushArrayString(lua_State *L, int key, const char *value);
void luaLE_pushArrayObject(lua_State *L,
                           int key,
                           const char *objectName,
                           const void *object);
void luaLE_pushObject(lua_State *L, const char *objectName, const void *object);
void luaLE_getModuleFunction(lua_State *L,
                             const char *module,
                             const char *function);
void luaLE_addObjectMethods(lua_State *L, const luaL_Reg *l);
void luaLE_postFunctionCleanUp(lua_State *L);
void luaLE_dumpstack(lua_State *L);
bool luaLE_functionExists(const char *module, const char *function);
bool luaLE_checkBoolean(lua_State *L, int idx);
void luaLE_getModuleFunction(const char *module, const char *function);
