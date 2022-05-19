#include "luaTransport.h"
#include "luaLE.h"
#include "MidiInputCallback.h"

int luaopen_transport(lua_State *L)
{
    luaL_newlib(L, transport_functions);
    return 1;
}

int transport_enable(lua_State *L)
{
    lua_settop(L, 0);

    logMessage("transport enabled");

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

    logMessage("transport disabled");

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
    midi_onSingleByte("onClock", midiInput);
}

void transport_onMidiStart(MidiInput midiInput)
{
    midi_onSingleByte("onStart", midiInput);
}

void transport_onMidiStop(MidiInput midiInput)
{
    midi_onSingleByte("onStop", midiInput);
}

void transport_onMidiContinue(MidiInput midiInput)
{
    midi_onSingleByte("onContinue", midiInput);
}

void transport_onMidiSongSelect(MidiInput midiInput, uint8_t songNumber)
{
    midi_onTwoBytes("onSongSelect", midiInput, songNumber);
}

void transport_onMidiSongPosition(MidiInput midiInput, int position)
{
    midi_onTwoBytes("onSongPosition", midiInput, position);
}
