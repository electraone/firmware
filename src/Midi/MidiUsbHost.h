#pragma once

#include "MidiBase.h"
#include "USBDevice.h"

// \todo Move to the class as static members.
extern USBDevice USBDevices[];
extern USBHost usbHost;

class MidiUsbHost : public MidiBase
{
public:
    MidiUsbHost() : activePort(0)
    {
    }

    ~MidiUsbHost() = default;

    void send(uint8_t port,
              MidiMessage::Type type,
              uint8_t channel,
              uint8_t data1,
              uint8_t data2) const override
    {
        if (USBDevices[port].midiDevice != NULL) {
            USBDevices[port].midiDevice->send(
                static_cast<uint8_t>(type), data1, data2, channel, port);
        }
    }

    void sendControlChange(uint8_t port,
                           uint8_t parameterNumber,
                           uint8_t value,
                           uint8_t channel) const override
    {
        if (USBDevices[port].midiDevice != NULL) {
            USBDevices[port].midiDevice->sendControlChange(
                parameterNumber, value, channel, port);
        }
    }

    void sendNoteOn(uint8_t port,
                    uint8_t channel,
                    uint8_t noteNumber,
                    uint8_t velocity) const override
    {
        if (USBDevices[port].midiDevice != NULL) {
            USBDevices[port].midiDevice->sendNoteOn(
                noteNumber, velocity, channel, port);
        }
    }

    void sendNoteOff(uint8_t port,
                     uint8_t channel,
                     uint8_t noteNumber,
                     uint8_t velocity) const override
    {
        if (USBDevices[port].midiDevice != NULL) {
            USBDevices[port].midiDevice->sendNoteOff(
                noteNumber, velocity, channel, port);
        }
    }

    void sendStart(uint8_t port) const override
    {
        if (USBDevices[port].midiDevice != NULL) {
            USBDevices[port].midiDevice->sendRealTime(usbMIDI.Start, port);
        }
    }

    void sendStop(uint8_t port) const override
    {
        if (USBDevices[port].midiDevice != NULL) {
            USBDevices[port].midiDevice->sendRealTime(usbMIDI.Stop, port);
        }
    }

    void sendTuneRequest(uint8_t port) const override
    {
        if (USBDevices[port].midiDevice != NULL) {
            USBDevices[port].midiDevice->sendTuneRequest(port);
        }
    }

    void sendProgramChange(uint8_t port,
                           uint8_t channel,
                           uint8_t programNumber) const override
    {
        if (USBDevices[port].midiDevice != NULL) {
            USBDevices[port].midiDevice->sendProgramChange(
                programNumber, channel, port);
        }
    }

    void sendSysEx(uint8_t port,
                   uint8_t *sysexData,
                   uint16_t sysexDataLength) const override
    {
        if (USBDevices[port].midiDevice != NULL) {
            USBDevices[port].midiDevice->sendSysEx(
                sysexDataLength, sysexData, false, port);
        }
    }

    void sendPitchBend(uint8_t port,
                       uint8_t channel,
                       uint16_t value) const override
    {
        if (USBDevices[port].midiDevice != NULL) {
            USBDevices[port].midiDevice->sendPitchBend(value, channel, port);
        }
    }

    void sendAfterTouchPoly(uint8_t port,
                            uint8_t channel,
                            uint8_t noteNumber,
                            uint8_t pressure) const override
    {
        if (USBDevices[port].midiDevice != NULL) {
            USBDevices[port].midiDevice->sendPolyPressure(
                noteNumber, pressure, channel, port);
        }
    }

    void sendAfterTouchChannel(uint8_t port,
                               uint8_t channel,
                               uint8_t pressure) const override
    {
        if (USBDevices[port].midiDevice != NULL) {
            USBDevices[port].midiDevice->sendAfterTouch(
                pressure, channel, port);
        }
    }

    void sendClock(uint8_t port) const override
    {
        if (USBDevices[port].midiDevice != NULL) {
            USBDevices[port].midiDevice->sendRealTime(usbMIDI.Clock, port);
        }
    }

    void sendContinue(uint8_t port) const override
    {
        if (USBDevices[port].midiDevice != NULL) {
            USBDevices[port].midiDevice->sendRealTime(usbMIDI.Continue, port);
        }
    }

    void sendActiveSensing(uint8_t port) const override
    {
        if (USBDevices[port].midiDevice != NULL) {
            USBDevices[port].midiDevice->sendRealTime(usbMIDI.ActiveSensing,
                                                      port);
        }
    }

    void sendSystemReset(uint8_t port) const override
    {
        if (USBDevices[port].midiDevice != NULL) {
            USBDevices[port].midiDevice->sendRealTime(usbMIDI.SystemReset,
                                                      port);
        }
    }

    void sendSongSelect(uint8_t port, uint8_t song) const override
    {
        if (USBDevices[port].midiDevice != NULL) {
            USBDevices[port].midiDevice->sendSongSelect(song, port);
        }
    }

    void sendSongPosition(uint8_t port, uint16_t beats) const override
    {
        if (USBDevices[port].midiDevice != NULL) {
            USBDevices[port].midiDevice->sendSongPosition(beats, port);
        }
    }

    void readNoAction(uint8_t port) const override
    {
        if (USBDevices[port].midiDevice != NULL) {
            USBDevices[port].midiDevice->readNoAction();
        }
    }

    bool read(uint8_t port) override
    {
        activePort = port;
        if (USBDevices[port].midiDevice != NULL) {
            return (USBDevices[port].midiDevice->read());
        }
        return (false);
    }

    uint8_t getChannel(uint8_t port) const override
    {
        if (USBDevices[port].midiDevice != NULL) {
            return (USBDevices[port].midiDevice->getChannel());
        }
        return (0);
    }

    uint8_t getType(uint8_t port) const override
    {
        if (USBDevices[port].midiDevice != NULL) {
            return (USBDevices[port].midiDevice->getType());
        }
        return (0);
    }

    uint8_t getData1(uint8_t port) const override
    {
        if (USBDevices[port].midiDevice != NULL) {
            return (USBDevices[port].midiDevice->getData1());
        }
        return (0);
    }

    uint8_t getData2(uint8_t port) const override
    {
        if (USBDevices[port].midiDevice != NULL) {
            return (USBDevices[port].midiDevice->getData2());
        }
        return (0);
    }

    uint8_t getActivePort(void) const override
    {
        return (activePort);
    }

    void initialise(void) const override
    {
        delay(10);
        usbHost.begin();

        // \todo No partial SysEx callback handler available.
    }

private:
    uint8_t activePort;
};
