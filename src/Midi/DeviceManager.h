#pragma once

#include "MidiMessage.h"
#include "MidiInterface.h"

class MidiInputCallback;

class DeviceManager
{
public:
    DeviceManager();
    ~DeviceManager() = default;

    void addMidiInputDeviceCallback(MidiInputCallback *newCallbackConsumer);
    void handleIncomingMidiMessage(const MidiInput &midiInput,
                                   const MidiMessage &midiMessage);

private:
    std::vector<MidiInputCallback *> consumers;
};
