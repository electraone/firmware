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
 * @file luaBridge.h
 *
 * @brief A compatibility interface for Lua.
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "lauxlib.h"
#include "lua.h"
#include "lualib.h"

int lua_compat_exists(const char *filename);
int lua_compat_fopen(const char *filename);
void lua_compat_fclose();
char lua_compat_getc();
int lua_compat_feof();
size_t lua_compat_fread(void *ptr, size_t size, size_t count);
int lua_compat_ferror();
void lua_compat_printf(char *format, ...);
void lua_compat_print(const char *s);

#ifdef __cplusplus
}
#endif
