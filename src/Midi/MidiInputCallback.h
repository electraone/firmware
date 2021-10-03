#pragma once

#include "MidiMessage.h"
#include "DeviceManager.h"

/**
 * @brief Receives incoming MIDI messages from Electra's MIDI interfaces.
 */
class MidiInputCallback
{
public:
    MidiInputCallback();
    ~MidiInputCallback() = default;

    /**
     * Gets called for all incoming MIDI messages besides Active sensing,
     * Clock, and SysEx.
     *
     * @param midiInput information about the MIDI message origin.
     * @param midiMessage received MIDI message.
     */
    virtual void handleIncomingMidiMessage(const MidiInput &midiInput,
                                           const MidiMessage &midiMessage) = 0;

    /** Optional callbacks handlers
     *
     */
    static void (*onMidiClockCallback)(MidiInput midiInput);
    static void (*onMidiStartCallback)(MidiInput midiInput);
    static void (*onMidiStopCallback)(MidiInput midiInput);
    static void (*onMidiContinueCallback)(MidiInput midiInput);
    static void (*onMidiActiveSensingCallback)(MidiInput midiInput);
    static void (*onMidiSystemResetCallback)(MidiInput midiInput);
    static void (*onMidiTuneRequestCallback)(MidiInput midiInput);
    static void (*onMidiProgramChangeCallback)(MidiInput midiInput,
                                               uint8_t channel,
                                               uint8_t programNumber);
    static void (*onMidiAfterTouchChannelCallback)(MidiInput midiInput,
                                                   uint8_t channel,
                                                   uint8_t pressure);
    static void (*onMidiSongSelectCallback)(MidiInput midiInput,
                                            uint8_t songNumber);
    static void (*onMidiPitchBendCallback)(MidiInput midiInput,
                                           uint8_t channel,
                                           int value);
    static void (*onMidiSongPositionCallback)(MidiInput midiInput,
                                              int position);
    static void (*onMidiControlChangeCallback)(MidiInput midiInput,
                                               uint8_t channel,
                                               uint8_t controllerNumber,
                                               uint8_t value);
    static void (*onMidiNoteOnCallback)(MidiInput midiInput,
                                        uint8_t channel,
                                        uint8_t noteNumber,
                                        uint8_t velocity);
    static void (*onMidiNoteOffCallback)(MidiInput midiInput,
                                         uint8_t channel,
                                         uint8_t noteNumber,
                                         uint8_t velocity);
    static void (*onMidiAfterTouchPolyCallback)(MidiInput midiInput,
                                                uint8_t channel,
                                                uint8_t noteNumber,
                                                uint8_t pressure);
    static void (*onMidiSysexCallback)(MidiInput midiInput,
                                       MidiMessage midiMessage);

    static DeviceManager deviceManager;
};
