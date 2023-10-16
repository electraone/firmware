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
 * @file MidiAll.h
 *
 * @brief A MIDI interface for all available MIDI interfaces.
 */

#pragma once

#include "MidiBase.h"

class MidiAll : public MidiBase
{
public:
    MidiAll();
    ~MidiAll() = default;

    void send(uint8_t port,
              MidiMessage::Type type,
              uint8_t channel,
              uint8_t data1,
              uint8_t data2) const override;
    void send(uint8_t port, MidiMessage &message) override;
    void send(uint8_t port, SysexBlock &sysexBlock) override;
    void sendControlChange(uint8_t port,
                           uint8_t parameterNumber,
                           uint8_t value,
                           uint8_t channel) const override;
    void sendNoteOn(uint8_t port,
                    uint8_t channel,
                    uint8_t noteNumber,
                    uint8_t velocity) const override;
    void sendNoteOff(uint8_t port,
                     uint8_t channel,
                     uint8_t noteNumber,
                     uint8_t velocity) const override;
    void sendStart(uint8_t port) const override;
    void sendStop(uint8_t port) const override;
    void sendTuneRequest(uint8_t port) const override;
    void sendProgramChange(uint8_t port,
                           uint8_t channel,
                           uint8_t programNumber) const override;
    void sendSysEx(uint8_t port, SysexBlock &sysexBlock) override;
    void sendSysEx(uint8_t port,
                   uint8_t *sysexData,
                   uint16_t sysexDataLength) const override;
    void sendSysExPartial(uint8_t port,
                          const uint8_t *sysexData,
                          uint16_t sysexDataLength,
                          bool complete) const;
    void sendPitchBend(uint8_t port,
                       uint8_t channel,
                       uint16_t value) const override;
    void sendAfterTouchPoly(uint8_t port,
                            uint8_t channel,
                            uint8_t noteNumber,
                            uint8_t pressure) const override;
    void sendAfterTouchChannel(uint8_t port,
                               uint8_t channel,
                               uint8_t pressure) const override;
    void sendClock(uint8_t port) const override;
    void sendContinue(uint8_t port) const override;
    void sendActiveSensing(uint8_t port) const override;
    void sendSystemReset(uint8_t port) const override;
    void sendSongSelect(uint8_t port, uint8_t song) const override;
    void sendSongPosition(uint8_t port, uint16_t beats) const override;
    void readNoAction(uint8_t port) const override;
    bool read(uint8_t port) override;
    uint8_t getChannel(uint8_t port) const override;
    uint8_t getType(uint8_t port) const override;
    uint8_t getData1(uint8_t port) const override;
    uint8_t getData2(uint8_t port) const override;
    uint8_t getActivePort(void) const override;
    void initialise(void) const override;
    void enableThru(uint8_t port, bool shouldBeEnabled) override;

private:
};
