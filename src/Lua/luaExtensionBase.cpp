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
 * @file luaLE.cpp
 *
 * @brief A collection of E1 specific Lua functions.
 */

#include "luaExtensionBase.h"
#include "SysexBlock.h"
#include "System.h"

extern bool lua_windowRepaintEnabled;

static const luaL_Reg eoslibs[] = { { "midi", luaopen_midi },
                                    { "window", luaopen_window },
                                    { "controller", luaopen_controller },
                                    { "timer", luaopen_timer },
                                    { "transport", luaopen_transport },
                                    { NULL, NULL } };

void luaLE_openEoslibs(lua_State *L, const luaL_Reg *libs)
{
    const luaL_Reg *lib;
    /* "require" functions from 'loadedlibs' and set results to global table */
    for (lib = libs; lib->func; lib++) {
        luaL_requiref(L, lib->name, lib->func, 1);
        lua_pop(L, 1); /* remove lib */
    }
}

lua_State *initLua(void)
{
    lua_State *lua = nullptr;
    // Enable printf/sprintf to print floats
    asm(".global _printf_float");

    lua = luaL_newstate(); /* opens Lua */
    luaL_openlibs(lua); /* opens the standard libraries */
    luaLE_openEoslibs(lua, eoslibs);

    // register userdate objects
    sysexBlock_register(lua);

    // register global variables
    loadGlobalVariables(lua);

    return (lua);
}

void closeLua(void)
{
    if (L) {
        lua_close(L);
        L = nullptr;
    }
}

void loadLuaModule(const char *filename)
{
    // Clear the stack
    lua_settop(L, 0);

    int error = luaL_dofile(L, filename);

    if (error) {
        System::logger.write(
            LOG_LUA,
            "loadLuaModule: error loading file: filename=%s, error=%s",
            filename,
            lua_tostring(L, -1));
        lua_pop(L, 1); /* pop error message from the stack */
    } else {
        System::logger.write(
            LOG_LUA,
            "loadLuaModule: Lua extension file initialized: file=%s",
            filename);
    }
}

void runLuaFunction(const char *functionName)
{
    lua_getglobal(L, functionName);

    if (lua_isfunction(L, -1)) {
        if (lua_pcall(L, 0, 0, 0) != 0) {
            System::logger.write(LOG_LUA,
                                 "error running function '%s': %s",
                                 functionName,
                                 lua_tostring(L, -1));
        }
    }
}

void runLuaString(const char *commandText)
{
    lua_settop(L, 0);

    int error = luaL_dostring(L, commandText);

    if (error) {
        System::logger.write(LOG_LUA,
                             "runLuaString: error loading file: error=%s",
                             lua_tostring(L, -1));
        lua_pop(L, 1); /* pop error message from the stack */
    }
}

void executeElectraLua(const char *filename)
{
    lua_windowRepaintEnabled = false;
    System::logger.write(LOG_LUA, "loadLua debug output:");
    System::logger.write(LOG_LUA, "---- START ----");
    loadLuaModule(filename);
    System::logger.write(LOG_LUA, "----- END -----");
    lua_windowRepaintEnabled = true;
}

bool isLuaValid(const char *filename)
{
    // @todo do the Lua source validation here
    return (Hardware::sdcard.exists(filename));
}

void loadGlobalVariables(lua_State *L)
{
    // colours
    lua_pushnumber(L, 0xffffff);
    lua_setglobal(L, "WHITE");
    lua_pushnumber(L, 0xF45C51);
    lua_setglobal(L, "RED");
    lua_pushnumber(L, 0xF49500);
    lua_setglobal(L, "ORANGE");
    lua_pushnumber(L, 0x529DEC);
    lua_setglobal(L, "BLUE");
    lua_pushnumber(L, 0x03A598);
    lua_setglobal(L, "GREEN");
    lua_pushnumber(L, 0xC44795);
    lua_setglobal(L, "PURPLE");

    // midi interfaces
    lua_pushnumber(L, 0);
    lua_setglobal(L, "MIDIIO");
    lua_pushnumber(L, 1);
    lua_setglobal(L, "USBDEV");
    lua_pushnumber(L, 2);
    lua_setglobal(L, "USBHOST");

    // midi ports
    lua_pushnumber(L, 0);
    lua_setglobal(L, "PORT_1");
    lua_pushnumber(L, 1);
    lua_setglobal(L, "PORT_2");
    lua_pushnumber(L, 2);
    lua_setglobal(L, "PORT_CTRL");

    // Change origins
    lua_pushnumber(L, 0);
    lua_setglobal(L, "INTERNAL");
    lua_pushnumber(L, 1);
    lua_setglobal(L, "MIDI");
    lua_pushnumber(L, 2);
    lua_setglobal(L, "LUA");

    // hardware buttons
    lua_pushnumber(L, 1);
    lua_setglobal(L, "BUTTON_1");
    lua_pushnumber(L, 2);
    lua_setglobal(L, "BUTTON_2");
    lua_pushnumber(L, 3);
    lua_setglobal(L, "BUTTON_3");
    lua_pushnumber(L, 4);
    lua_setglobal(L, "BUTTON_4");
    lua_pushnumber(L, 5);
    lua_setglobal(L, "BUTTON_5");
    lua_pushnumber(L, 6);
    lua_setglobal(L, "BUTTON_6");

    // pots
    lua_pushnumber(L, 1);
    lua_setglobal(L, "POT_1");
    lua_pushnumber(L, 2);
    lua_setglobal(L, "POT_2");
    lua_pushnumber(L, 3);
    lua_setglobal(L, "POT_3");
    lua_pushnumber(L, 4);
    lua_setglobal(L, "POT_4");
    lua_pushnumber(L, 5);
    lua_setglobal(L, "POT_5");
    lua_pushnumber(L, 6);
    lua_setglobal(L, "POT_6");
    lua_pushnumber(L, 7);
    lua_setglobal(L, "POT_7");
    lua_pushnumber(L, 8);
    lua_setglobal(L, "POT_8");
    lua_pushnumber(L, 9);
    lua_setglobal(L, "POT_9");
    lua_pushnumber(L, 10);
    lua_setglobal(L, "POT_10");
    lua_pushnumber(L, 11);
    lua_setglobal(L, "POT_11");
    lua_pushnumber(L, 12);
    lua_setglobal(L, "POT_12");

    // touch points
    lua_pushnumber(L, 1);
    lua_setglobal(L, "TOUCH_POINT_1");
    lua_pushnumber(L, 2);
    lua_setglobal(L, "TOUCH_POINT_2");
    lua_pushnumber(L, 3);
    lua_setglobal(L, "TOUCH_POINT_3");
    lua_pushnumber(L, 4);
    lua_setglobal(L, "TOUCH_POINT_4");
    lua_pushnumber(L, 5);
    lua_setglobal(L, "TOUCH_POINT_5");

    // MIDI message types
    lua_pushnumber(L, 176);
    lua_setglobal(L, "CONTROL_CHANGE");
    lua_pushnumber(L, 144);
    lua_setglobal(L, "NOTE_ON");
    lua_pushnumber(L, 128);
    lua_setglobal(L, "NOTE_OFF");
    lua_pushnumber(L, 192);
    lua_setglobal(L, "PROGRAM_CHANGE");
    lua_pushnumber(L, 160);
    lua_setglobal(L, "POLY_PRESSURE");
    lua_pushnumber(L, 208);
    lua_setglobal(L, "CHANNEL_PRESSURE");
    lua_pushnumber(L, 224);
    lua_setglobal(L, "PITCH_BEND");
    lua_pushnumber(L, 248);
    lua_setglobal(L, "CLOCK");
    lua_pushnumber(L, 250);
    lua_setglobal(L, "START");
    lua_pushnumber(L, 252);
    lua_setglobal(L, "STOP");
    lua_pushnumber(L, 251);
    lua_setglobal(L, "CONTINUE");
    lua_pushnumber(L, 254);
    lua_setglobal(L, "ACTIVE_SENSING");
    lua_pushnumber(L, 255);
    lua_setglobal(L, "RESET");
    lua_pushnumber(L, 243);
    lua_setglobal(L, "SONG_SELECT");
    lua_pushnumber(L, 242);
    lua_setglobal(L, "SONG_POSITION");
    lua_pushnumber(L, 246);
    lua_setglobal(L, "TUNE_REQUEST");
    lua_pushnumber(L, 241);
    lua_setglobal(L, "TIME_CODE_QUARTER_FRAME");
    lua_pushnumber(L, 240);
    lua_setglobal(L, "SYSEX");

    // Electra Parameter types
    lua_pushnumber(L, 0);
    lua_setglobal(L, "PT_VIRTUAL");
    lua_pushnumber(L, 1);
    lua_setglobal(L, "PT_CC7");
    lua_pushnumber(L, 2);
    lua_setglobal(L, "PT_CC14");
    lua_pushnumber(L, 3);
    lua_setglobal(L, "PT_NRPN");
    lua_pushnumber(L, 4);
    lua_setglobal(L, "PT_RPN");
    lua_pushnumber(L, 5);
    lua_setglobal(L, "PT_NOTE");
    lua_pushnumber(L, 6);
    lua_setglobal(L, "PT_PROGRAM");
    lua_pushnumber(L, 7);
    lua_setglobal(L, "PT_SYSEX");
    lua_pushnumber(L, 8);
    lua_setglobal(L, "PT_START");
    lua_pushnumber(L, 9);
    lua_setglobal(L, "PT_STOP");
    lua_pushnumber(L, 10);
    lua_setglobal(L, "PT_TUNE");
    lua_pushnumber(L, 11);
    lua_setglobal(L, "PT_ATPOLY");
    lua_pushnumber(L, 12);
    lua_setglobal(L, "PT_ATCHANNEL");
    lua_pushnumber(L, 13);
    lua_setglobal(L, "PT_PITCHBEND");
    lua_pushnumber(L, 14);
    lua_setglobal(L, "PT_SPP");
    lua_pushnumber(L, 15);
    lua_setglobal(L, "PT_RELCC");
    lua_pushnumber(L, 16);
    lua_setglobal(L, "PT_NONE");

    // MIDI interfaces
    lua_pushnumber(L, 0);
    lua_setglobal(L, "MIDI_IO");
    lua_pushnumber(L, 1);
    lua_setglobal(L, "USB_DEV");
    lua_pushnumber(L, 2);
    lua_setglobal(L, "USB_HOST");

    // Bounds
    lua_pushnumber(L, 1);
    lua_setglobal(L, "X");
    lua_pushnumber(L, 2);
    lua_setglobal(L, "Y");
    lua_pushnumber(L, 3);
    lua_setglobal(L, "WIDTH");
    lua_pushnumber(L, 4);
    lua_setglobal(L, "HEIGHT");

    // Control sets
    lua_pushnumber(L, 1);
    lua_setglobal(L, "CONTROL_SET_1");
    lua_pushnumber(L, 2);
    lua_setglobal(L, "CONTROL_SET_2");
    lua_pushnumber(L, 3);
    lua_setglobal(L, "CONTROL_SET_3");

    // Events
    lua_pushnumber(L, 0);
    lua_setglobal(L, "NONE");
    lua_pushnumber(L, 1);
    lua_setglobal(L, "PAGES");
    lua_pushnumber(L, 2);
    lua_setglobal(L, "CONTROL_SETS");
    lua_pushnumber(L, 4);
    lua_setglobal(L, "USB_HOST");
    lua_pushnumber(L, 8);
    lua_setglobal(L, "POTS");
    lua_pushnumber(L, 16);
    lua_setglobal(L, "TOUCH");
    lua_pushnumber(L, 32);
    lua_setglobal(L, "BUTTONS");
    lua_pushnumber(L, 64);
    lua_setglobal(L, "WINDOWS");

    // Variants
    lua_pushnumber(L, 0);
    lua_setglobal(L, "VT_DEFAULT");
    lua_pushnumber(L, 1);
    lua_setglobal(L, "VT_HIGHLIGHTED");
}
