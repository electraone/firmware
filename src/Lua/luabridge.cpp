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

#include "luabridge.h"
#include "System.h"

// \todo PermanentStorage should be used instead.
extern SdFs SD;

// \todo temp fix. Lua needs its own class.
#include "System.h"
lua_State *L = System::L;

// Compatibility functions for Serial output and SdFat file operations
extern "C" {

StdioStream lua_temp_stdio_file;

int _gettimeofday(struct timeval *tv, void *tzvp __attribute__((unused)))
{
    return (0);
}

clock_t _times(struct tms *buffer)
{
    return (0);
}

int lua_compat_exists(const char *filename)
{
    return (Hardware::sdcard.exists(filename) ? 1 : 0);
}

int lua_compat_fopen(const char *filename)
{
    // Serial << "-- sd fopen: " << filename << "\n";
    return (lua_temp_stdio_file.fopen(filename, "r") ? 1 : 0);
}

void lua_compat_fclose()
{
    // Serial << "-- sd fclose\n";
    lua_temp_stdio_file.fclose();
}

char lua_compat_getc()
{
    // Serial << "-- sd getc\n";
    return (lua_temp_stdio_file.getc());
}

int lua_compat_feof()
{
    // Serial << "-- sd feof\n";
    return (lua_temp_stdio_file.feof());
}

size_t lua_compat_fread(void *ptr, size_t size, size_t count)
{
    // Serial << "-- sd fread -- size: " << size << " count: " << count << "\n";
    return (lua_temp_stdio_file.fread(ptr, size, count));
}

int lua_compat_ferror()
{
    // Serial << "-- sd ferror\n";
    return (lua_temp_stdio_file.ferror());
}

void lua_compat_printf(char *format, ...)
{
    char buf[256];
    va_list ap;

    va_start(ap, format);
    System::logger.write(LOG_ERROR, buf, sizeof(buf), format, ap);
    va_end(ap);
}

void lua_compat_print(const char *s)
{
    if (s && (s[0] != '\n') && (s[0] != '\0')) {
        System::logger.write(LOG_ERROR, "lua: %s", s);
    }
}

} // extern C