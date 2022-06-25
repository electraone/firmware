#pragma once

#include "MidiMessage.h"

class MidiBase
{
public:
    virtual ~MidiBase() = default;

    virtual void send(uint8_t port,
                      MidiMessage::Type type,
                      uint8_t channel,
                      uint8_t data1,
                      uint8_t data2) const = 0;
    virtual void sendControlChange(uint8_t port,
                                   uint8_t parameterNumber,
                                   uint8_t value,
                                   uint8_t channel) const = 0;
    virtual void sendNoteOn(uint8_t port,
                            uint8_t channel,
                            uint8_t noteNumber,
                            uint8_t velocity) const = 0;
    virtual void sendNoteOff(uint8_t port,
                             uint8_t channel,
                             uint8_t noteNumber,
                             uint8_t velocity) const = 0;
    virtual void sendStart(uint8_t port) const = 0;
    virtual void sendStop(uint8_t port) const = 0;
    virtual void sendTuneRequest(uint8_t port) const = 0;
    virtual void sendProgramChange(uint8_t port,
                                   uint8_t channel,
                                   uint8_t programNumber) const = 0;
    virtual void sendSysEx(uint8_t port,
                           uint8_t *sysexData,
                           uint16_t sysexDataLength) const = 0;
    virtual void
        sendPitchBend(uint8_t port, uint8_t channel, uint16_t value) const = 0;
    virtual void sendAfterTouchPoly(uint8_t port,
                                    uint8_t channel,
                                    uint8_t noteNumber,
                                    uint8_t pressure) const = 0;
    virtual void sendAfterTouchChannel(uint8_t port,
                                       uint8_t channel,
                                       uint8_t pressure) const = 0;
    virtual void sendClock(uint8_t port) const = 0;
    virtual void sendContinue(uint8_t port) const = 0;
    virtual void sendActiveSensing(uint8_t port) const = 0;
    virtual void sendSystemReset(uint8_t port) const = 0;
    virtual void sendSongSelect(uint8_t port, uint8_t song) const = 0;
    virtual void sendSongPosition(uint8_t port, uint16_t beats) const = 0;

    virtual void readNoAction(uint8_t port) const = 0;
    virtual bool read(uint8_t port) = 0;

    virtual uint8_t getChannel(uint8_t port) const = 0;
    virtual uint8_t getType(uint8_t port) const = 0;
    virtual uint8_t getData1(uint8_t port) const = 0;
    virtual uint8_t getData2(uint8_t port) const = 0;
    virtual uint8_t getActivePort(void) const = 0;

    virtual void initialise(void) const = 0;
};
