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
 * @file MidiMessageTransport.h
 *
 * @brief A wrapper class to exchange MidiMessage between queues.
 */

#pragma once

#include "MidiMessage.h"
#include "MidiInterface.h"

enum class Direction { in, out };

class MidiOutput;

struct MidiMessageTransport : public MidiJack, public MidiMessage {
public:
    MidiMessageTransport()
        : MidiJack(MidiInterface::Type::None, 0), id(micros()), invalid(false)
    {
    }

    MidiMessageTransport(MidiJack newJack, MidiMessage newMessage)
        : MidiJack(newJack),
          MidiMessage(newMessage),
          id(micros()),
          invalid(false)
    {
    }

    /** Constructor. */
    MidiMessageTransport(MidiInterface::Type newInterface,
                         uint8_t newPort,
                         MidiMessage newMessage)
        : MidiJack(newInterface, newPort),
          MidiMessage(newMessage),
          id(micros()),
          invalid(false)
    {
    }

    /** Constructor. */
    MidiMessageTransport(MidiInterface::Type newInterface,
                         uint8_t newPort,
                         uint8_t newChannel,
                         Type newType,
                         uint8_t newData1,
                         uint8_t newData2)
        : MidiJack(newInterface, newPort),
          MidiMessage(newChannel, newType, newData1, newData2),
          id(micros()),
          invalid(false)
    {
    }

    ~MidiMessageTransport() = default;

    uint32_t id;
    bool invalid;
};
