#include "MidiIo.h"

// \todo move to the MidiIo class.
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI1);
MIDI_CREATE_INSTANCE(HardwareSerial, Serial2, MIDI2);

constexpr midi::MidiInterface<HardwareSerial> *MidiIo::midiDINInterfaces[2];
