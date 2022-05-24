#include "MidiInterface.h"

MidiIo MidiInterface::midiIo;
MidiUsbDev MidiInterface::midiUsbDev;
MidiUsbHost MidiInterface::midiUsbHost;

const std::array<MidiInterface::Type, 3> MidiInterface::allInterfaceTypes = {
    MidiInterface::Type::MidiIo,
    MidiInterface::Type::MidiUsbDev,
    MidiInterface::Type::MidiUsbHost
};
