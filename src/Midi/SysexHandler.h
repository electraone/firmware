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
 * @file SysexHandler.h
 *
 * @brief A callback to process incoming Sysex data on user ports.
 */

#pragma once

#include "App.h"
#include "SysexBlock.h"
#include "MidiInterface.h"
#include "MidiMessage.h"
#include "MidiInputCallback.h"
#include "helpers.h"

class SysexHandler
{
public:
    SysexHandler(MidiInterface::Type newMidiInterface, uint8_t newPort)
        : midiInterface(newMidiInterface), port(newPort), packetNr(0)
    {
    }

    virtual ~SysexHandler() = default;

    virtual void process(const uint8_t *sysExData,
                         uint16_t sysExSize,
                         bool complete,
                         int8_t portOverride = -1)
    {
        App *app = App::get();

        if (packetNr == 0) {
            sysexBlock = SysexBlock(app->sysexPool.openMemoryBlock());
        }

        sysexBlock.writeBytes(sysExData, sysExSize);
        packetNr++;

        if (complete) {
            sysexBlock.close();

            if (app->enableMidi) {
                processGeneralSysex(midiInterface, port, sysexBlock);
            }

            packetNr = 0;
        }
    }

protected:
    static void processGeneralSysex(MidiInterface::Type &midiInterface,
                                    uint8_t port,
                                    const SysexBlock &sysexBlock)
    {
        System::logger.write(LOG_ERROR,
                             "processGeneralSysex: manufacturer %X",
                             sysexBlock.getManufacturerSysexId());
        MidiInput midiInput(midiInterface, port);
        MidiMessage midiMessage(sysexBlock);

        if (MidiInputCallback::onMidiSysexCallback) {
            MidiInputCallback::onMidiSysexCallback(midiInput, midiMessage);
        }

        if (MidiInputCallback::onMidiMessageCallback) {
            MidiInputCallback::onMidiMessageCallback(midiInput, midiMessage);
        }

        MidiInputCallback::deviceManager.handleIncomingMidiMessage(midiInput,
                                                                   midiMessage);
    }

    MidiInterface::Type midiInterface;
    uint8_t port;
    uint32_t packetNr;
    SysexBlock sysexBlock;
};
