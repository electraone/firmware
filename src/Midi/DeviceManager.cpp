#include "DeviceManager.h"
#include "MidiInputCallback.h"

DeviceManager::DeviceManager()
{
}

void DeviceManager::addMidiInputDeviceCallback(
    MidiInputCallback *newCallbackConsumer)
{
    consumers.push_back(newCallbackConsumer);
}

void DeviceManager::handleIncomingMidiMessage(const MidiInput &midiInput,
                                              const MidiMessage &midiMessage)
{
    for (auto &consumer : consumers) {
        consumer->handleIncomingMidiMessage(midiInput, midiMessage);
    }
}
