#include "MidiAll.h"
#include "MidiOutput.h"

MidiAll::MidiAll()
{
}

void MidiAll::send(uint8_t port,
                   MidiMessage::Type type,
                   uint8_t channel,
                   uint8_t data1,
                   uint8_t data2) const
{
    for (const auto &interfaceType : MidiInterface::allInterfaceTypes) {
        MidiOutput::send(interfaceType, port, type, channel, data1, data2);
    }
}

void MidiAll::send(uint8_t port, MidiMessage &message)
{
    for (const auto &interfaceType : MidiInterface::allInterfaceTypes) {
        if (message.getType() == MidiMessage::Type::SystemExclusive) {
            MidiOutput::send(interfaceType, port, message.getSysExBlock());
        } else {
            MidiOutput::send(interfaceType, port, message);
        }
    }
}

void MidiAll::send(uint8_t port, SysexBlock &sysexBlock)
{
    for (const auto &interfaceType : MidiInterface::allInterfaceTypes) {
        MidiOutput::send(interfaceType, port, sysexBlock);
    }
}

void MidiAll::sendControlChange(uint8_t port,
                                uint8_t parameterNumber,
                                uint8_t value,
                                uint8_t channel) const
{
    for (const auto &interfaceType : MidiInterface::allInterfaceTypes) {
        MidiOutput::sendControlChange(
            interfaceType, port, channel, parameterNumber, value);
    }
}

void MidiAll::sendNoteOn(uint8_t port,
                         uint8_t channel,
                         uint8_t noteNumber,
                         uint8_t velocity) const
{
    for (const auto &interfaceType : MidiInterface::allInterfaceTypes) {
        MidiOutput::sendNoteOn(
            interfaceType, port, channel, noteNumber, velocity);
    }
}

void MidiAll::sendNoteOff(uint8_t port,
                          uint8_t channel,
                          uint8_t noteNumber,
                          uint8_t velocity) const
{
    for (const auto &interfaceType : MidiInterface::allInterfaceTypes) {
        MidiOutput::sendNoteOff(
            interfaceType, port, channel, noteNumber, velocity);
    }
}

void MidiAll::sendStart(uint8_t port) const
{
    for (const auto &interfaceType : MidiInterface::allInterfaceTypes) {
        MidiOutput::sendStart(interfaceType, port);
    }
}

void MidiAll::sendStop(uint8_t port) const
{
    for (const auto &interfaceType : MidiInterface::allInterfaceTypes) {
        MidiOutput::sendStop(interfaceType, port);
    }
}

void MidiAll::sendTuneRequest(uint8_t port) const
{
    for (const auto &interfaceType : MidiInterface::allInterfaceTypes) {
        MidiOutput::sendTuneRequest(interfaceType, port);
    }
}

void MidiAll::sendProgramChange(uint8_t port,
                                uint8_t channel,
                                uint8_t programNumber) const
{
    for (const auto &interfaceType : MidiInterface::allInterfaceTypes) {
        MidiOutput::sendProgramChange(
            interfaceType, port, channel, programNumber);
    }
}

void MidiAll::sendSysEx(uint8_t port, SysexBlock &sysexBlock)
{
    for (const auto &interfaceType : MidiInterface::allInterfaceTypes) {
        MidiOutput::sendSysEx(interfaceType, port, sysexBlock);
    }
}

void MidiAll::sendSysEx(uint8_t port,
                        uint8_t *sysexData,
                        uint16_t sysexDataLength) const
{
    for (const auto &interfaceType : MidiInterface::allInterfaceTypes) {
        MidiOutput::sendSysEx(interfaceType, port, sysexData, sysexDataLength);
    }
}

void MidiAll::sendSysExPartial(uint8_t port,
                               const uint8_t *sysexData,
                               uint16_t sysexDataLength,
                               bool complete) const
{
    for (const auto &interfaceType : MidiInterface::allInterfaceTypes) {
        MidiOutput::sendSysExPartial(
            interfaceType, port, sysexData, sysexDataLength, complete);
    }
}

void MidiAll::sendPitchBend(uint8_t port, uint8_t channel, uint16_t value) const
{
    for (const auto &interfaceType : MidiInterface::allInterfaceTypes) {
        MidiOutput::sendPitchBend(interfaceType, port, channel, value);
    }
}

void MidiAll::sendAfterTouchPoly(uint8_t port,
                                 uint8_t channel,
                                 uint8_t noteNumber,
                                 uint8_t pressure) const
{
    for (const auto &interfaceType : MidiInterface::allInterfaceTypes) {
        MidiOutput::sendAfterTouchPoly(
            interfaceType, port, channel, noteNumber, pressure);
    }
}

void MidiAll::sendAfterTouchChannel(uint8_t port,
                                    uint8_t channel,
                                    uint8_t pressure) const
{
    for (const auto &interfaceType : MidiInterface::allInterfaceTypes) {
        MidiOutput::sendAfterTouchChannel(
            interfaceType, port, channel, pressure);
    }
}

void MidiAll::sendClock(uint8_t port) const
{
    for (const auto &interfaceType : MidiInterface::allInterfaceTypes) {
        MidiOutput::sendClock(interfaceType, port);
    }
}

void MidiAll::sendContinue(uint8_t port) const
{
    for (const auto &interfaceType : MidiInterface::allInterfaceTypes) {
        MidiOutput::sendContinue(interfaceType, port);
    }
}

void MidiAll::sendActiveSensing(uint8_t port) const
{
    for (const auto &interfaceType : MidiInterface::allInterfaceTypes) {
        MidiOutput::sendActiveSensing(interfaceType, port);
    }
}

void MidiAll::sendSystemReset(uint8_t port) const
{
    for (const auto &interfaceType : MidiInterface::allInterfaceTypes) {
        MidiOutput::sendSystemReset(interfaceType, port);
    }
}

void MidiAll::sendSongSelect(uint8_t port, uint8_t song) const
{
    for (const auto &interfaceType : MidiInterface::allInterfaceTypes) {
        MidiOutput::sendSongSelect(interfaceType, port, song);
    }
}

void MidiAll::sendSongPosition(uint8_t port, uint16_t beats) const
{
    for (const auto &interfaceType : MidiInterface::allInterfaceTypes) {
        MidiOutput::sendSongPosition(interfaceType, port, beats);
    }
}

void MidiAll::readNoAction(uint8_t port) const
{
}

bool MidiAll::read(uint8_t port)
{
    return (false);
}

uint8_t MidiAll::getChannel(uint8_t port) const
{
    return (0);
}

uint8_t MidiAll::getType(uint8_t port) const
{
    return (0);
}

uint8_t MidiAll::getData1(uint8_t port) const
{
    return (0);
}

uint8_t MidiAll::getData2(uint8_t port) const
{
    return (0);
}

uint8_t MidiAll::getActivePort(void) const
{
    return (0);
}

void MidiAll::initialise(void) const
{
}

void MidiAll::enableThru(uint8_t port, bool shouldBeEnabled)
{
    MidiOutput::enableThru(MidiInterface::Type::MidiIo, port, shouldBeEnabled);
}
