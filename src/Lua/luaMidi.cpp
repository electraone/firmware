#include "luaMidi.h"
#include "MidiOutput.h"
#include "luaLE.h"
#include "System.h"

int luaopen_midi(lua_State *L)
{
    luaL_newlib(L, midi_functions);
    return 1;
}

/**
 * sends a MidiMessage out
 * @params a midiMessage table
 */
int midi_sendMessage(lua_State *L)
{
    MidiInterface::Type interface = MidiInterface::Type::MidiAll;
    int portIndex = 1;
    int tableIndex = 2;

    if (lua_gettop(L) == 3) { // if number of parameters is 3
        interface = static_cast<MidiInterface::Type>(luaL_checkinteger(L, 1));
        portIndex = 2;
        tableIndex = 3;
    }

    int port = luaL_checkinteger(L, portIndex);

    // get the message table
    luaL_checktype(L, tableIndex, LUA_TTABLE);

    // parse type
    lua_getfield(L, tableIndex, "type");
    int type = luaL_checkinteger(L, -1);

    // process type specific parts
    if (type == (uint8_t)MidiMessage::Type::ControlChange) {
        lua_getfield(L, tableIndex, "channel");
        lua_getfield(L, tableIndex, "controllerNumber");
        lua_getfield(L, tableIndex, "value");

        int channel = luaL_checkinteger(L, -3);
        int data1 = luaL_checkinteger(L, -2);
        int data2 = luaL_checkinteger(L, -1);

        MidiOutput::sendControlChange(interface, port, channel, data1, data2);
    } else if (type == (uint8_t)MidiMessage::Type::ProgramChange) {
        lua_getfield(L, tableIndex, "channel");
        lua_getfield(L, tableIndex, "programNumber");

        int channel = luaL_checkinteger(L, -2);
        int data1 = luaL_checkinteger(L, -1);

        MidiOutput::sendProgramChange(interface, port, channel, data1);
    } else if (type == (uint8_t)MidiMessage::Type::NoteOn) {
        lua_getfield(L, tableIndex, "channel");
        lua_getfield(L, tableIndex, "noteNumber");
        lua_getfield(L, tableIndex, "velocity");

        int channel = luaL_checkinteger(L, -3);
        int data1 = luaL_checkinteger(L, -2);
        int data2 = luaL_checkinteger(L, -1);

        MidiOutput::sendNoteOn(interface, port, channel, data1, data2);
    } else if (type == (uint8_t)MidiMessage::Type::NoteOff) {
        lua_getfield(L, tableIndex, "channel");
        lua_getfield(L, tableIndex, "noteNumber");
        lua_getfield(L, tableIndex, "velocity");

        int channel = luaL_checkinteger(L, -3);
        int data1 = luaL_checkinteger(L, -2);
        int data2 = luaL_checkinteger(L, -1);

        MidiOutput::sendNoteOff(interface, port, channel, data1, data2);
    } else if (type == (uint8_t)MidiMessage::Type::PitchBend) {
        lua_getfield(L, tableIndex, "channel");
        lua_getfield(L, tableIndex, "value");

        int channel = luaL_checkinteger(L, -2);
        int value = luaL_checkinteger(L, -1);

        MidiOutput::sendPitchBend(interface, port, channel, value);
    } else if (type == (uint8_t)MidiMessage::Type::AfterTouchPoly) {
        lua_getfield(L, tableIndex, "channel");
        lua_getfield(L, tableIndex, "noteNumber");
        lua_getfield(L, tableIndex, "pressure");

        int channel = luaL_checkinteger(L, -3);
        int data1 = luaL_checkinteger(L, -2);
        int data2 = luaL_checkinteger(L, -1);

        MidiOutput::sendAfterTouchPoly(interface, port, channel, data1, data2);
    } else if (type == (uint8_t)MidiMessage::Type::AfterTouchChannel) {
        lua_getfield(L, tableIndex, "channel");
        lua_getfield(L, tableIndex, "pressure");

        int channel = luaL_checkinteger(L, -2);
        int data1 = luaL_checkinteger(L, -1);

        MidiOutput::sendAfterTouchChannel(interface, port, channel, data1);
    } else if (type == (uint8_t)MidiMessage::Type::Clock) {
        MidiOutput::sendClock(interface, port);
    } else if (type == (uint8_t)MidiMessage::Type::Start) {
        MidiOutput::sendStart(interface, port);
    } else if (type == (uint8_t)MidiMessage::Type::Stop) {
        MidiOutput::sendStop(interface, port);
    } else if (type == (uint8_t)MidiMessage::Type::Continue) {
        MidiOutput::sendContinue(interface, port);
    } else if (type == (uint8_t)MidiMessage::Type::ActiveSensing) {
        MidiOutput::sendActiveSensing(interface, port);
    } else if (type == (uint8_t)MidiMessage::Type::SystemReset) {
        MidiOutput::sendSystemReset(interface, port);
    } else if (type == (uint8_t)MidiMessage::Type::SongSelect) {
        lua_getfield(L, tableIndex, "songNumber");
        int songNumber = luaL_checkinteger(L, -1);

        MidiOutput::sendSongSelect(interface, port, songNumber);
    } else if (type == (uint8_t)MidiMessage::Type::SongPosition) {
        lua_getfield(L, tableIndex, "position");
        int position = luaL_checkinteger(L, -1);

        MidiOutput::sendSongPosition(interface, port, position);
    } else if (type == (uint8_t)MidiMessage::Type::TuneRequest) {
        MidiOutput::sendTuneRequest(interface, port);
    } else {
        return (luaL_error(L, "type %c is not supported", type));
    }

    return (0);
}

/**
 * sends out a Continuous Controller MIDI message
 * @param port an identifier of the port number
 * @param channel a MIDI channel
 * @param parameterNumber a numeric identifier of the continuous controller
 * @param value a numeric value to send
 * @returns status and error code if present
 */
int midi_sendControlChange(lua_State *L)
{
    int nextIndex = 0;
    int interface = getInterface(L, &nextIndex);
    int port = luaL_checkinteger(L, nextIndex++);
    int channel = luaL_checkinteger(L, nextIndex++);
    int controllerNumber = luaL_checkinteger(L, nextIndex++);
    int value = luaL_checkinteger(L, nextIndex);

    checkInterface(interface);
    checkPort(port);
    checkChannel(channel);
    check7bit(controllerNumber, "controllerNumber");
    check7bit(value, "value");

    MidiOutput::sendControlChange(static_cast<MidiInterface::Type>(interface),
                                  port,
                                  channel,
                                  controllerNumber,
                                  value);

    return (0);
}

/**
 * sends out a Note On MIDI message
 * @param port an identifier of the port number
 * @param channel a MIDI channel
 * @param noteNumber a numeric identifier of the MIDI note
 * @param velocity a velocity of the note
 * @returns status and error code if present
 */
int midi_sendNoteOn(lua_State *L)
{
    int nextIndex = 0;
    int interface = getInterface(L, &nextIndex);
    int port = luaL_checkinteger(L, nextIndex++);
    int channel = luaL_checkinteger(L, nextIndex++);
    int noteNumber = luaL_checkinteger(L, nextIndex++);
    int velocity = luaL_checkinteger(L, nextIndex);

    checkInterface(interface);
    checkPort(port);
    checkChannel(channel);
    check7bit(noteNumber, "noteNumber");
    check7bit(velocity, "velocity");

    MidiOutput::sendNoteOn(static_cast<MidiInterface::Type>(interface),
                           port,
                           channel,
                           noteNumber,
                           velocity);

    return (0);
}

/**
 * sends out a Note Off MIDI message
 * @param port an identifier of the port number
 * @param channel a MIDI channel
 * @param noteNumber a numeric identifier of the MIDI note
 * @param velocity a velocity of the note
 * @returns status and error code if present
 */
int midi_sendNoteOff(lua_State *L)
{
    int nextIndex = 0;
    int interface = getInterface(L, &nextIndex);
    int port = luaL_checkinteger(L, nextIndex++);
    int channel = luaL_checkinteger(L, nextIndex++);
    int noteNumber = luaL_checkinteger(L, nextIndex++);
    int velocity = luaL_checkinteger(L, nextIndex);

    checkInterface(interface);
    checkPort(port);
    checkChannel(channel);
    check7bit(noteNumber, "noteNumber");
    check7bit(velocity, "velocity");

    MidiOutput::sendNoteOff(static_cast<MidiInterface::Type>(interface),
                            port,
                            channel,
                            noteNumber,
                            velocity);

    return (0);
}

/**
 * sends out a Pitch Bend MIDI message
 * @param port an identifier of the port number
 * @param channel a MIDI channel
 * @param value a rate of the pitch bend applied
 * @returns status and error code if present
 */
int midi_sendPitchBend(lua_State *L)
{
    int nextIndex = 0;
    int interface = getInterface(L, &nextIndex);
    int port = luaL_checkinteger(L, nextIndex++);
    int channel = luaL_checkinteger(L, nextIndex++);
    int value = luaL_checkinteger(L, nextIndex);

    checkInterface(interface);
    checkPort(port);
    checkChannel(channel);
    check14bit(value, "value");

    MidiOutput::sendPitchBend(
        static_cast<MidiInterface::Type>(interface), port, channel, value);

    return (0);
}

/**
 * sends out a Polyphonic Aftertouch MIDI message
 * @param port an identifier of the port number
 * @param channel a MIDI channel
 * @param noteNumber a numeric identifier of the MIDI note
 * @param pressure a velocity of the note
 * @returns status and error code if present
 */
int midi_sendAfterTouchPoly(lua_State *L)
{
    int nextIndex = 0;
    int interface = getInterface(L, &nextIndex);
    int port = luaL_checkinteger(L, nextIndex++);
    int channel = luaL_checkinteger(L, nextIndex++);
    int noteNumber = luaL_checkinteger(L, nextIndex++);
    int pressure = luaL_checkinteger(L, nextIndex);

    checkInterface(interface);
    checkPort(port);
    checkChannel(channel);
    check7bit(noteNumber, "noteNumber");
    check7bit(pressure, "pressure");

    MidiOutput::sendAfterTouchPoly(static_cast<MidiInterface::Type>(interface),
                                   port,
                                   channel,
                                   noteNumber,
                                   pressure);

    return (0);
}

/**
 * sends out a Channel Aftertouch MIDI message
 * @param port an identifier of the port number
 * @param channel a MIDI channel
 * @param pressure a velocity of the note
 * @returns status and error code if present
 */
int midi_sendAfterTouchChannel(lua_State *L)
{
    int nextIndex = 0;
    int interface = getInterface(L, &nextIndex);
    int port = luaL_checkinteger(L, nextIndex++);
    int channel = luaL_checkinteger(L, nextIndex++);
    int pressure = luaL_checkinteger(L, nextIndex);

    checkInterface(interface);
    checkPort(port);
    checkChannel(channel);
    check7bit(pressure, "pressure");

    MidiOutput::sendAfterTouchChannel(
        static_cast<MidiInterface::Type>(interface), port, channel, pressure);

    return (0);
}

/**
 * sends out a MIDI RealTime Clock message
 * @param port an identifier of the port number
 * @returns status and error code if present
 */
int midi_sendClock(lua_State *L)
{
    int nextIndex = 0;
    int interface = getInterface(L, &nextIndex);
    int port = luaL_checkinteger(L, nextIndex);

    checkInterface(interface);
    checkPort(port);

    MidiOutput::sendClock(static_cast<MidiInterface::Type>(interface), port);

    return (0);
}

/**
 * sends out a MIDI RealTime Start message
 * @param port an identifier of the port number
 * @returns status and error code if present
 */
int midi_sendStart(lua_State *L)
{
    int nextIndex = 0;
    int interface = getInterface(L, &nextIndex);
    int port = luaL_checkinteger(L, nextIndex);

    checkInterface(interface);
    checkPort(port);

    MidiOutput::sendStart(static_cast<MidiInterface::Type>(interface), port);

    return (0);
}

/**
 * sends out a MIDI RealTime Stop message
 * @param port an identifier of the port number
 * @returns status and error code if present
 */
int midi_sendStop(lua_State *L)
{
    int nextIndex = 0;
    int interface = getInterface(L, &nextIndex);
    int port = luaL_checkinteger(L, nextIndex);

    checkInterface(interface);
    checkPort(port);

    MidiOutput::sendStop(static_cast<MidiInterface::Type>(interface), port);

    return (0);
}

/**
 * sends out a MIDI RealTime Continue message
 * @param port an identifier of the port number
 * @returns status and error code if present
 */
int midi_sendContinue(lua_State *L)
{
    int nextIndex = 0;
    int interface = getInterface(L, &nextIndex);
    int port = luaL_checkinteger(L, nextIndex);

    checkInterface(interface);
    checkPort(port);

    MidiOutput::sendContinue(static_cast<MidiInterface::Type>(interface), port);

    return (0);
}

/**
 * sends out a MIDI RealTime Active Sensing message
 * @param port an identifier of the port number
 * @returns status and error code if present
 */
int midi_sendActiveSensing(lua_State *L)
{
    int nextIndex = 0;
    int interface = getInterface(L, &nextIndex);
    int port = luaL_checkinteger(L, nextIndex);

    checkInterface(interface);
    checkPort(port);

    MidiOutput::sendActiveSensing(static_cast<MidiInterface::Type>(interface),
                                  port);

    return (0);
}

/**
 * sends out a MIDI RealTime System Reset message
 * @param port an identifier of the port number
 * @returns status and error code if present
 */
int midi_sendSystemReset(lua_State *L)
{
    int nextIndex = 0;
    int interface = getInterface(L, &nextIndex);
    int port = luaL_checkinteger(L, nextIndex);

    checkInterface(interface);
    checkPort(port);

    MidiOutput::sendSystemReset(static_cast<MidiInterface::Type>(interface),
                                port);

    return (0);
}

/**
 * sends out a MIDI Song Select message
 * @param port an identifier of the port number
 * @param songNumber an identifier of the song
 * @returns status and error code if present
 */
int midi_sendSongSelect(lua_State *L)
{
    int nextIndex = 0;
    int interface = getInterface(L, &nextIndex);
    int port = luaL_checkinteger(L, nextIndex++);
    int song = luaL_checkinteger(L, nextIndex);

    checkInterface(interface);
    checkPort(port);
    check7bit(song, "song");

    MidiOutput::sendSongSelect(
        static_cast<MidiInterface::Type>(interface), port, song);

    return (0);
}

/**
 * sends out a MIDI Song Select message
 * @param port an identifier of the port number
 * @param songNumber an identifier of the song
 * @returns status and error code if present
 */
int midi_sendSongPosition(lua_State *L)
{
    int nextIndex = 0;
    int interface = getInterface(L, &nextIndex);
    int port = luaL_checkinteger(L, nextIndex++);
    int position = luaL_checkinteger(L, nextIndex);

    checkInterface(interface);
    checkPort(port);
    check14bit(position, "position");

    MidiOutput::sendSongPosition(
        static_cast<MidiInterface::Type>(interface), port, position);

    return (0);
}

/**
 * sends out a MIDI Tune Request message
 * @param port an identifier of the port number
 * @returns status and error code if present
 */
int midi_sendTuneRequest(lua_State *L)
{
    int nextIndex = 0;
    int interface = getInterface(L, &nextIndex);
    int port = luaL_checkinteger(L, nextIndex);

    checkInterface(interface);
    checkPort(port);

    MidiOutput::sendTuneRequest(static_cast<MidiInterface::Type>(interface),
                                port);

    return (0);
}

/**
 * sends out a Program Change MIDI message
 * @param port an identifier of the port number
 * @param channel a MIDI channel
 * @param programNumber a numeric identifier of the MIDI program
 * @returns status and error code if present
 */
int midi_sendProgramChange(lua_State *L)
{
    int nextIndex = 0;
    int interface = getInterface(L, &nextIndex);
    int port = luaL_checkinteger(L, nextIndex++);
    int channel = luaL_checkinteger(L, nextIndex++);
    int programNumber = luaL_checkinteger(L, nextIndex);

    checkInterface(interface);
    checkPort(port);
    checkChannel(channel);
    check7bit(programNumber, "programNumber");

    MidiOutput::sendProgramChange(static_cast<MidiInterface::Type>(interface),
                                  port,
                                  channel,
                                  programNumber);

    return (0);
}

/**
 * sends out a MIDI SysEx message
 * @param port an identifier of the port number
 * @param array of SysEx bytes. F0 and F7 shall not be included
 * @returns status and error code if present
 */
int midi_sendSysEx(lua_State *L)
{
    int interface = LUA_MIDI_INTERFACE_MIDI_ALL;
    int portIndex = 1;
    int tableIndex = 2;

    if (lua_gettop(L) == 3) { // if number of parameters is 3
        interface = luaL_checkinteger(L, 1);
        portIndex = 2;
        tableIndex = 3;
    }

    int port = luaL_checkinteger(L, portIndex);

    checkInterface(interface);
    checkPort(port);

    luaL_checktype(L, tableIndex, LUA_TTABLE);

    /* table is in the stack at index 1 */
    lua_pushnil(L);

    uint8_t buffer[LUA_MAX_SYSEX_SIZE];
    int i = 0;

    while ((lua_next(L, tableIndex) != 0) && (i < LUA_MAX_SYSEX_SIZE)) {
        buffer[i] = luaL_checkinteger(L, -1);
        lua_pop(L, 1);
        i++;
    }

    MidiOutput::sendSysEx(
        static_cast<MidiInterface::Type>(interface), port, buffer, i);

    return (0);
}

/**
 * sends out a NRPN MIDI message
 * @param port an identifier of the port number
 * @param channel a MIDI channel
 * @param parameterNumber a numeric identifier of the NPRN parameter
 * @param value a numeric value to send
 * @param lsbFirst a switch to swap an order of LSB and MSB bytes
 * @returns status and error code if present
 */
int midi_sendNrpn(lua_State *L)
{
    int nextIndex = 0;
    int interface = getInterface(L, &nextIndex);
    int port = luaL_checkinteger(L, nextIndex++);
    int channel = luaL_checkinteger(L, nextIndex++);
    int parameterNumber = luaL_checkinteger(L, nextIndex++);
    int value = luaL_checkinteger(L, nextIndex++);
    int lsbFirst = luaL_optinteger(L, nextIndex++, 0);
    int resetRpn = luaL_optinteger(L, nextIndex, 1);

    checkInterface(interface);
    checkPort(port);
    checkChannel(channel);
    check14bit(parameterNumber, "parameterNumber");
    check14bit(value, "value");
    checkBoolean(lsbFirst, "lsbFirst");
    checkBoolean(resetRpn, "resetRpn");

    MidiOutput::sendNrpn(static_cast<MidiInterface::Type>(interface),
                         port,
                         channel,
                         parameterNumber,
                         value,
                         lsbFirst,
                         resetRpn);

    return (0);
}

/**
 * sends out a RPN MIDI message
 * @param port an identifier of the port number
 * @param channel a MIDI channel
 * @param parameterNumber a numeric identifier of the PRN parameter
 * @param value a numeric value to send
 * @returns status and error code if present
 */
int midi_sendRpn(lua_State *L)
{
    int nextIndex = 0;
    int interface = getInterface(L, &nextIndex);
    int port = luaL_checkinteger(L, nextIndex++);
    int channel = luaL_checkinteger(L, nextIndex++);
    int parameterNumber = luaL_checkinteger(L, nextIndex++);
    int value = luaL_checkinteger(L, nextIndex);

    checkInterface(interface);
    checkPort(port);
    checkChannel(channel);
    check14bit(parameterNumber, "parameterNumber");
    check14bit(value, "value");

    MidiOutput::sendRpn(static_cast<MidiInterface::Type>(interface),
                        port,
                        channel,
                        parameterNumber,
                        value);

    return (0);
}

/**
 * sends out a 14-bit Continuous Controller MIDI message
 * @param port an identifier of the port number
 * @param channel a MIDI channel
 * @param parameterNumber a numeric identifier of the CC14 parameter
 * @param value a numeric value to send
 * @param lsbFirst a switch to swap an order of LSB and MSB bytes
 * @returns status and error code if present
 */
int midi_sendControlChange14Bit(lua_State *L)
{
    int nextIndex = 0;
    int interface = getInterface(L, &nextIndex);
    int port = luaL_checkinteger(L, nextIndex++);
    int channel = luaL_checkinteger(L, nextIndex++);
    int controllerNumber = luaL_checkinteger(L, nextIndex++);
    int value = luaL_checkinteger(L, nextIndex++);
    int lsbFirst = luaL_optinteger(L, nextIndex, 0);

    checkInterface(interface);
    checkPort(port);
    checkChannel(channel);
    check14bit(controllerNumber, "controllerNumber");
    check14bit(value, "value");
    checkBoolean(lsbFirst, "lsbFirst");

    MidiOutput::sendControlChange14Bit(
        static_cast<MidiInterface::Type>(interface),
        port,
        channel,
        controllerNumber,
        value,
        lsbFirst);

    return (0);
}

void midi_onSingleByte(const char *module,
                       const char *function,
                       MidiInput midiInput)
{
    luaLE_getModuleFunction(module, function);

    if (lua_isfunction(L, -1)) {
        lua_newtable(L);
        luaLE_pushTableInteger(
            L, "interface", (uint8_t)midiInput.getInterfaceType());
        luaLE_pushTableInteger(L, "port", midiInput.getPort());

        if (lua_pcall(L, 1, 0, 0) != 0) {
            System::logger.write(ERROR,
                                 "error running function '%s': %s",
                                 function,
                                 lua_tostring(L, -1));
        }
    } else {
        luaLE_handleNonexistentFunction(L, function);
    }
}

void midi_onTwoBytes(const char *module,
                     const char *function,
                     MidiInput midiInput,
                     int data1)
{
    luaLE_getModuleFunction(module, function);

    if (lua_isfunction(L, -1)) {
        lua_newtable(L);
        luaLE_pushTableInteger(
            L, "interface", (uint8_t)midiInput.getInterfaceType());
        luaLE_pushTableInteger(L, "port", midiInput.getPort());
        lua_pushnumber(L, data1);

        if (lua_pcall(L, 2, 0, 0) != 0) {
            System::logger.write(ERROR,
                                 "error running function '%s': %s",
                                 function,
                                 lua_tostring(L, -1));
        }
    } else {
        luaLE_handleNonexistentFunction(L, function);
    }
}

void midi_onTwoBytesWithChannel(const char *module,
                                const char *function,
                                MidiInput midiInput,
                                uint8_t channel,
                                int data1)
{
    luaLE_getModuleFunction(module, function);

    if (lua_isfunction(L, -1)) {
        lua_newtable(L);
        luaLE_pushTableInteger(
            L, "interface", (uint8_t)midiInput.getInterfaceType());
        luaLE_pushTableInteger(L, "port", midiInput.getPort());
        lua_pushnumber(L, channel);
        lua_pushnumber(L, data1);

        if (lua_pcall(L, 3, 0, 0) != 0) {
            System::logger.write(ERROR,
                                 "error running function '%s': %s",
                                 function,
                                 lua_tostring(L, -1));
        }
    } else {
        luaLE_handleNonexistentFunction(L, function);
    }
}

void midi_onThreeBytesWithChannel(const char *module,
                                  const char *function,
                                  MidiInput midiInput,
                                  uint8_t channel,
                                  uint8_t data1,
                                  uint8_t data2)
{
    luaLE_getModuleFunction(module, function);

    if (lua_isfunction(L, -1)) {
        lua_newtable(L);
        luaLE_pushTableInteger(
            L, "interface", (uint8_t)midiInput.getInterfaceType());
        luaLE_pushTableInteger(L, "port", midiInput.getPort());
        lua_pushnumber(L, channel);
        lua_pushnumber(L, data1);
        lua_pushnumber(L, data2);

        if (lua_pcall(L, 4, 0, 0) != 0) {
            System::logger.write(ERROR,
                                 "error running function '%s': %s",
                                 function,
                                 lua_tostring(L, -1));
        }
    } else {
        luaLE_handleNonexistentFunction(L, function);
    }
}

void midi_onMidiSysex(MidiInput &midiInput, SysexBlock &sysexBlock)
{
    luaLE_getModuleFunction("midi", "onSysex");

    if (lua_isfunction(L, -1)) {
        lua_newtable(L);
        luaLE_pushTableInteger(
            L, "interface", (uint8_t)midiInput.getInterfaceType());
        luaLE_pushTableInteger(L, "port", midiInput.getPort());
        luaLE_pushObject(L, "SysexBlock", &sysexBlock);

        if (lua_pcall(L, 2, 0, 0) != 0) {
            System::logger.write(ERROR,
                                 "error running function 'onSysex': %s",
                                 lua_tostring(L, -1));
        }
    } else {
        luaLE_handleNonexistentFunction(L, "onSysex");
    }
}

int getInterface(lua_State *L, int *nextIndex)
{
    int numParameters = lua_gettop(L);
    int interface = LUA_MIDI_INTERFACE_MIDI_ALL;

    if (numParameters == 5) {
        interface = luaL_checkinteger(L, -numParameters);
        numParameters--;
    }

    *nextIndex = -numParameters;
    return (interface);
}
