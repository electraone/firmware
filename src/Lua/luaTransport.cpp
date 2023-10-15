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

#include "luaTransport.h"
#include "luaExtensionBase.h"
#include "MidiInputCallback.h"

int luaopen_transport(lua_State *L)
{
    luaL_newlib(L, transport_functions);
    return 1;
}

int transport_enable(lua_State *L)
{
    lua_settop(L, 0);

    System::logger.write(LOG_INFO, "lua: transport enabled");

    MidiInputCallback::onMidiClockCallback = &transport_onMidiClock;
    MidiInputCallback::onMidiStartCallback = &transport_onMidiStart;
    MidiInputCallback::onMidiStopCallback = &transport_onMidiStop;
    MidiInputCallback::onMidiContinueCallback = &transport_onMidiContinue;
    MidiInputCallback::onMidiSongSelectCallback = &transport_onMidiSongSelect;
    MidiInputCallback::onMidiSongPositionCallback =
        &transport_onMidiSongPosition;

    return (0);
}

int transport_disable(lua_State *L)
{
    lua_settop(L, 0);

    System::logger.write(LOG_INFO, "lua: transport disabled");

    MidiInputCallback::onMidiClockCallback = nullptr;
    MidiInputCallback::onMidiStartCallback = nullptr;
    MidiInputCallback::onMidiStopCallback = nullptr;
    MidiInputCallback::onMidiContinueCallback = nullptr;
    MidiInputCallback::onMidiSongSelectCallback = nullptr;
    MidiInputCallback::onMidiSongPositionCallback = nullptr;

    return (0);
}

int transport_isEnabled(lua_State *L)
{
    lua_pushboolean(
        L, (MidiInputCallback::onMidiClockCallback == nullptr) ? false : true);
    return (1);
}

void transport_onMidiClock(MidiInput midiInput)
{
    midi_onSingleByte(L, "transport", "onClock", midiInput);
}

void transport_onMidiStart(MidiInput midiInput)
{
    midi_onSingleByte(L, "transport", "onStart", midiInput);
}

void transport_onMidiStop(MidiInput midiInput)
{
    midi_onSingleByte(L, "transport", "onStop", midiInput);
}

void transport_onMidiContinue(MidiInput midiInput)
{
    midi_onSingleByte(L, "transport", "onContinue", midiInput);
}

void transport_onMidiSongSelect(MidiInput midiInput, uint8_t songNumber)
{
    midi_onTwoBytes(L, "transport", "onSongSelect", midiInput, songNumber);
}

void transport_onMidiSongPosition(MidiInput midiInput, int position)
{
    midi_onTwoBytes(L, "transport", "onSongPosition", midiInput, position);
}
