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
