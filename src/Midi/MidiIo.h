#pragma once

#include "MidiBase.h"
#include <MIDI.h>

// \todo Move to the class as static members.
extern midi::MidiInterface<HardwareSerial> MIDI1;
extern midi::MidiInterface<HardwareSerial> MIDI2;

void handleMidiIoSysExPort0(const uint8_t *sysExData,
                            uint16_t sysExSize,
                            bool complete);
void handleMidiIoSysExPort1(const uint8_t *sysExData,
                            uint16_t sysExSize,
                            bool complete);

/*
 * set up DIN5 MIDI interfaces
 */

class MidiIo : public MidiBase
{
public:
    MidiIo() : activePort(0)
    {
    }

    ~MidiIo() = default;

    void sendControlChange(uint8_t port,
                           uint8_t parameterNumber,
                           uint8_t value,
                           uint8_t channel) const override
    {
        midiDINInterfaces[port]->sendControlChange(
            parameterNumber, value, channel);
    }

    void sendNoteOn(uint8_t port,
                    uint8_t channel,
                    uint8_t noteNumber,
                    uint8_t velocity) const override
    {
        midiDINInterfaces[port]->sendNoteOn(noteNumber, velocity, channel);
    }

    void sendNoteOff(uint8_t port,
                     uint8_t channel,
                     uint8_t noteNumber,
                     uint8_t velocity) const override
    {
        midiDINInterfaces[port]->sendNoteOff(noteNumber, velocity, channel);
    }

    void sendStart(uint8_t port) const override
    {
        midiDINInterfaces[port]->sendRealTime(midi::Start);
    }

    void sendStop(uint8_t port) const override
    {
        midiDINInterfaces[port]->sendRealTime(midi::Stop);
    }

    void sendTuneRequest(uint8_t port) const override
    {
        midiDINInterfaces[port]->sendTuneRequest();
    }

    void sendProgramChange(uint8_t port,
                           uint8_t channel,
                           uint8_t programNumber) const override
    {
        midiDINInterfaces[port]->sendProgramChange(programNumber, channel);
    }

    void sendSysEx(uint8_t port,
                   uint8_t *sysexData,
                   uint16_t sysexDataLength) const override
    {
        midiDINInterfaces[port]->sendSysEx(sysexDataLength, sysexData, false);
    }

    void sendPitchBend(uint8_t port,
                       uint8_t channel,
                       uint16_t value) const override
    {
        midiDINInterfaces[port]->sendPitchBend(value, channel);
    }

    void sendAfterTouchPoly(uint8_t port,
                            uint8_t channel,
                            uint8_t noteNumber,
                            uint8_t pressure) const override
    {
        midiDINInterfaces[port]->sendPolyPressure(
            noteNumber, pressure, channel);
    }

    void sendAfterTouchChannel(uint8_t port,
                               uint8_t channel,
                               uint8_t pressure) const override
    {
        midiDINInterfaces[port]->sendAfterTouch(pressure, channel);
    }

    void sendClock(uint8_t port) const override
    {
        midiDINInterfaces[port]->sendRealTime(midi::Clock);
    }

    void sendContinue(uint8_t port) const override
    {
        midiDINInterfaces[port]->sendRealTime(midi::Continue);
    }

    void sendActiveSensing(uint8_t port) const override
    {
        midiDINInterfaces[port]->sendRealTime(midi::ActiveSensing);
    }

    void sendSystemReset(uint8_t port) const override
    {
        midiDINInterfaces[port]->sendRealTime(midi::SystemReset);
    }

    void sendSongSelect(uint8_t port, uint8_t song) const override
    {
        midiDINInterfaces[port]->sendSongSelect(song);
    }

    void sendSongPosition(uint8_t port, uint16_t beats) const override
    {
        midiDINInterfaces[port]->sendSongPosition(beats);
    }

    void readNoAction(uint8_t port) const override
    {
        midiDINInterfaces[port]->readNoAction();
    }

    bool read(uint8_t port) override
    {
        activePort = port;
        return (midiDINInterfaces[port]->read());
    }

    uint8_t getChannel(uint8_t port) const override
    {
        return (midiDINInterfaces[port]->getChannel());
    }

    uint8_t getType(uint8_t port) const override
    {
        return (midiDINInterfaces[port]->getType());
    }

    uint8_t getData1(uint8_t port) const override
    {
        return (midiDINInterfaces[port]->getData1());
    }

    uint8_t getData2(uint8_t port) const override
    {
        return (midiDINInterfaces[port]->getData2());
    }

    uint8_t getActivePort(void) const
    {
        return (activePort);
    }

    void initialise(void) const override
    {
        midiDINInterfaces[0]->begin(MIDI_CHANNEL_OMNI);
        midiDINInterfaces[1]->begin(MIDI_CHANNEL_OMNI);

        midiDINInterfaces[0]->turnThruOff();
        midiDINInterfaces[1]->turnThruOff();

        midiDINInterfaces[0]->setHandleSystemExclusivePartial(
            handleMidiIoSysExPort0);
        midiDINInterfaces[1]->setHandleSystemExclusivePartial(
            handleMidiIoSysExPort1);
    }

private:
    uint8_t activePort;
    static constexpr midi::MidiInterface<HardwareSerial>
        *midiDINInterfaces[2] = { &MIDI1, &MIDI2 };
};
