#include "MidiUsbHost.h"

// \todo needs to be moved to the MidiUsbHost class
USBHost usbHost;
USBHub hub1(usbHost);
MIDIDevice midiDevice1(usbHost);
MIDIDevice midiDevice2(usbHost);

USBDriver *drivers[] = { &hub1, &midiDevice1, &midiDevice2 };

const char *driverNames[3] = { "HUB1", "MIDI1", "MIDI2" };
bool driverActive[3] = {};
USBDevice USBDevices[2];
