#pragma once

extern "C" {
#include "lauxlib.h"
#include "lua.h"
#include "lualib.h"
}

#include "helpers.h"
#include "MidiInterface.h"

int luaopen_transport(lua_State *L);

int transport_enable(lua_State *L);
int transport_disable(lua_State *L);
int transport_isEnabled(lua_State *L);

void transport_onMidiClock(MidiInput midiInput);
void transport_onMidiStart(MidiInput midiInput);
void transport_onMidiStop(MidiInput midiInput);
void transport_onMidiContinue(MidiInput midiInput);
void transport_onMidiSongSelect(MidiInput midiInput, uint8_t songNumber);
void transport_onMidiSongPosition(MidiInput midiInput, int position);

void onSingleByte(const char *function, MidiInput midiInput);
void onTwoBytes(const char *function, MidiInput midiInput, int data1);

static const luaL_Reg transport_functions[] = {
    { "enable", transport_enable },
    { "disable", transport_disable },
    { "isEnabled", transport_isEnabled },
    { NULL, NULL }
};
