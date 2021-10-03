#include "MidiInputCallback.h"

MidiInputCallback::MidiInputCallback()
{
    deviceManager.addMidiInputDeviceCallback(this);
}

void (*MidiInputCallback::onMidiClockCallback)(MidiInput midiInput) = nullptr;
void (*MidiInputCallback::onMidiStartCallback)(MidiInput midiInput) = nullptr;
void (*MidiInputCallback::onMidiStopCallback)(MidiInput midiInput) = nullptr;
void (*MidiInputCallback::onMidiContinueCallback)(MidiInput midiInput) =
    nullptr;
void (*MidiInputCallback::onMidiActiveSensingCallback)(MidiInput midiInput) =
    nullptr;
void (*MidiInputCallback::onMidiSystemResetCallback)(MidiInput midiInput) =
    nullptr;
void (*MidiInputCallback::onMidiTuneRequestCallback)(MidiInput midiInput) =
    nullptr;
void (*MidiInputCallback::onMidiProgramChangeCallback)(MidiInput midiInput,
                                                       uint8_t channel,
                                                       uint8_t programNumber) =
    nullptr;
void (*MidiInputCallback::onMidiAfterTouchChannelCallback)(MidiInput midiInput,
                                                           uint8_t channel,
                                                           uint8_t pressure) =
    nullptr;
void (*MidiInputCallback::onMidiSongSelectCallback)(MidiInput midiInput,
                                                    uint8_t songNumber) =
    nullptr;
void (*MidiInputCallback::onMidiPitchBendCallback)(MidiInput midiInput,
                                                   uint8_t channel,
                                                   int value) = nullptr;
void (*MidiInputCallback::onMidiSongPositionCallback)(MidiInput midiInput,
                                                      int position) = nullptr;
void (*MidiInputCallback::onMidiControlChangeCallback)(MidiInput midiInput,
                                                       uint8_t channel,
                                                       uint8_t controllerNumber,
                                                       uint8_t value) = nullptr;
void (*MidiInputCallback::onMidiNoteOnCallback)(MidiInput midiInput,
                                                uint8_t channel,
                                                uint8_t noteNumber,
                                                uint8_t velocity) = nullptr;
void (*MidiInputCallback::onMidiNoteOffCallback)(MidiInput midiInput,
                                                 uint8_t channel,
                                                 uint8_t noteNumber,
                                                 uint8_t velocity) = nullptr;
void (*MidiInputCallback::onMidiAfterTouchPolyCallback)(MidiInput midiInput,
                                                        uint8_t channel,
                                                        uint8_t noteNumber,
                                                        uint8_t pressure) =
    nullptr;
void (*MidiInputCallback::onMidiSysexCallback)(MidiInput midiInput,
                                               MidiMessage midiMessage) =
    nullptr;

DeviceManager MidiInputCallback::deviceManager;
