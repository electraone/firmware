#pragma once

#include "MidiMessage.h"
#include "MidiInterface.h"
#include "CircularBuffer.h"

void readCtrlMidi(void);
void readMidi(void);
void processMidi(void);
bool runRouteMessageCallback(MidiInput &midiInput, MidiMessage &midiMessage);
void runOptionalCallbacks(MidiInput midiInput,
                          uint8_t channel,
                          MidiMessage::Type type,
                          uint8_t data1,
                          uint8_t data2);
