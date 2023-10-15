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
 * @file luaTransport.h
 *
 * @brief Implements a Lua API for MIDI transport functions
 */

#pragma once

#include "luaIntegrationBase.h"
#include "MidiInterface.h"

int luaopen_transport(lua_State *L);

int transport_enable(lua_State *L);
int transport_disable(lua_State *L);
int transport_isEnabled(lua_State *L);

static const luaL_Reg transport_functions[] = {
    { "enable", transport_enable },
    { "disable", transport_disable },
    { "isEnabled", transport_isEnabled },
    { NULL, NULL }
};

void transport_onMidiClock(MidiInput midiInput);
void transport_onMidiStart(MidiInput midiInput);
void transport_onMidiStop(MidiInput midiInput);
void transport_onMidiContinue(MidiInput midiInput);
void transport_onMidiSongSelect(MidiInput midiInput, uint8_t songNumber);
void transport_onMidiSongPosition(MidiInput midiInput, int position);
