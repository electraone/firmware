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
 * @file MidiInterface.cpp
 *
 * @brief A definition of all available MIDI interfaces.
 */

#pragma once

#include "MidiIo.h"
#include "MidiUsbDev.h"
#include "MidiUsbHost.h"
#include "MidiAll.h"
#include <array>

class MidiInterface final
{
public:
    enum class Type {
        MidiIo = 0,
        MidiUsbDev = 1,
        MidiUsbHost = 2,
        MidiAll = 3,
        None = 4
    };

    static MidiBase *get(MidiInterface::Type interface)
    {
        if (interface == MidiInterface::Type::MidiIo) {
            return (&MidiInterface::midiIo);
        } else if (interface == MidiInterface::Type::MidiUsbDev) {
            return (&MidiInterface::midiUsbDev);
        } else if (interface == MidiInterface::Type::MidiUsbHost) {
            return (&MidiInterface::midiUsbHost);
        } else if (interface == MidiInterface::Type::MidiAll) {
            return (&MidiInterface::midiAll);
        } else {
            return (nullptr);
        }
    }

    static const char *getName(MidiInterface::Type interface)
    {
        if (interface == MidiInterface::Type::MidiIo) {
            return ("MidiIo");
        } else if (interface == MidiInterface::Type::MidiUsbDev) {
            return ("MidiUsbDev");
        } else if (interface == MidiInterface::Type::MidiUsbHost) {
            return ("MidiUsbHost");
        } else if (interface == MidiInterface::Type::MidiAll) {
            return ("MidiAll");
        } else {
            return ("Uknown");
        }
    }

    static const std::array<MidiInterface::Type, 3> allInterfaceTypes;

private:
    static MidiIo midiIo;
    static MidiUsbDev midiUsbDev;
    static MidiUsbHost midiUsbHost;
    static MidiAll midiAll;
};

class MidiJack
{
public:
    MidiJack(MidiInterface::Type newInterface, uint8_t newPort)
        : interface(newInterface), port(newPort)
    {
    }

    virtual ~MidiJack()
    {
    }

    virtual MidiInterface::Type getInterfaceType(void) const
    {
        return (interface);
    }

    virtual uint8_t getPort(void) const
    {
        return (port);
    }

protected:
    MidiInterface::Type interface;
    uint8_t port;
};

class MidiInput : public MidiJack
{
public:
    MidiInput(MidiInterface::Type newInterface, uint8_t newPort)
        : MidiJack(newInterface, newPort)
    {
    }
};
