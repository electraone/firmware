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
