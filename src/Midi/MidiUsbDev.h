#pragma once

#include "MidiBase.h"
#include <usb_midi.h>

// \todo Move to the class as static members. If possible.
extern usb_midi_class usbMIDI;

void handleMidiUsbDevSysEx(uint8_t cable,
                           const uint8_t *sysExData,
                           uint16_t sysExSize,
                           bool complete);

class MidiUsbDev : public MidiBase
{
public:
    MidiUsbDev() : activePort(0)
    {
    }

    ~MidiUsbDev() = default;

    void send(uint8_t port,
              MidiMessage::Type type,
              uint8_t channel,
              uint8_t data1,
              uint8_t data2) const override
    {
        usbMIDI.send(static_cast<uint8_t>(type), data1, data2, channel, port);
    }

    void send(uint8_t port, MidiMessage &message) override
    {
        usbMIDI.send(static_cast<uint8_t>(message.getType()),
                     message.getData1(),
                     message.getData2(),
                     message.getChannel(),
                     port);
    }

    void send(uint8_t port, SysexBlock &sysexBlock) override
    {
        sendSysEx(port, sysexBlock);
    }

    void sendControlChange(uint8_t port,
                           uint8_t parameterNumber,
                           uint8_t value,
                           uint8_t channel) const override
    {
        usbMIDI.sendControlChange(parameterNumber, value, channel, port);
    }

    void sendNoteOn(uint8_t port,
                    uint8_t channel,
                    uint8_t noteNumber,
                    uint8_t velocity) const override
    {
        usbMIDI.sendNoteOn(noteNumber, velocity, channel, port);
    }

    void sendNoteOff(uint8_t port,
                     uint8_t channel,
                     uint8_t noteNumber,
                     uint8_t velocity) const override
    {
        usbMIDI.sendNoteOff(noteNumber, velocity, channel, port);
    }

    void sendStart(uint8_t port) const override
    {
        usbMIDI.sendRealTime(usbMIDI.Start, port);
    }

    void sendStop(uint8_t port) const override
    {
        usbMIDI.sendRealTime(usbMIDI.Stop, port);
    }

    void sendTuneRequest(uint8_t port) const override
    {
        usbMIDI.sendTuneRequest(port);
    }

    void sendProgramChange(uint8_t port,
                           uint8_t channel,
                           uint8_t programNumber) const override
    {
        usbMIDI.sendProgramChange(programNumber, channel, port);
    }

    void sendSysEx(uint8_t port, SysexBlock &sysexBlock) override
    {
        uint8_t buffer[MemoryBlock::headerMaxSize];
        sysexBlock.seek(0);
        size_t readBytes = 0;
        while ((readBytes = sysexBlock.readBytes(buffer, sizeof(buffer)))
               != 0) {
            sendSysExPartial(port, buffer, readBytes, false);
        }
    }

    void sendSysEx(uint8_t port,
                   uint8_t *sysexData,
                   uint16_t sysexDataLength) const override
    {
        usbMIDI.sendSysEx(sysexDataLength, sysexData, false, port);
    }

    void sendSysExPartial(uint8_t port,
                          const uint8_t *sysexData,
                          uint16_t sysexDataLength,
                          bool complete) const override
    {
        usbMIDI.sendSysEx(sysexDataLength, sysexData, true, port);
    }

    void sendPitchBend(uint8_t port,
                       uint8_t channel,
                       uint16_t value) const override
    {
        usbMIDI.sendPitchBend(value, channel, port);
    }

    void sendAfterTouchPoly(uint8_t port,
                            uint8_t channel,
                            uint8_t noteNumber,
                            uint8_t pressure) const override
    {
        usbMIDI.sendPolyPressure(noteNumber, pressure, channel, port);
    }

    void sendAfterTouchChannel(uint8_t port,
                               uint8_t channel,
                               uint8_t pressure) const override
    {
        usbMIDI.sendAfterTouch(pressure, channel, port);
    }

    void sendClock(uint8_t port) const override
    {
        usbMIDI.sendRealTime(usbMIDI.Clock, port);
    }

    void sendContinue(uint8_t port) const override
    {
        usbMIDI.sendRealTime(usbMIDI.Continue, port);
    }

    void sendActiveSensing(uint8_t port) const override
    {
        usbMIDI.sendRealTime(usbMIDI.ActiveSensing, port);
    }

    void sendSystemReset(uint8_t port) const override
    {
        usbMIDI.sendRealTime(usbMIDI.SystemReset, port);
    }

    void sendSongSelect(uint8_t port, uint8_t song) const override
    {
        usbMIDI.sendSongSelect(song, port);
    }

    void sendSongPosition(uint8_t port, uint16_t beats) const override
    {
        usbMIDI.sendSongPosition(beats, port);
    }

    void readNoAction(uint8_t port) const override
    {
        if (port == 0) {
            usb_midi_read_sysex();
        }
    }

    bool read(uint8_t port) override
    {
        bool status = usbMIDI.read();

        if (status) {
            activePort = usbMIDI.getCable();
        }

        return (status);
    }

    uint8_t getChannel(uint8_t port) const override
    {
        return (usbMIDI.getChannel());
    }

    uint8_t getType(uint8_t port) const override
    {
        return (usbMIDI.getType());
    }

    uint8_t getData1(uint8_t port) const override
    {
        return (usbMIDI.getData1());
    }

    uint8_t getData2(uint8_t port) const override
    {
        return (usbMIDI.getData2());
    }

    uint8_t getActivePort(void) const override
    {
        return (activePort);
    }

    void initialise(void) const override
    {
        usbMIDI.setHandleSysEx(handleMidiUsbDevSysEx);
    }

    void enableThru(uint8_t port, bool shouldBeEnabled) override
    {
    }

private:
    uint8_t activePort;
};
