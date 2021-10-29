#include "TaskScanUSBHost.h"
#include "SysexCallbacks.h"
#include "System.h"
#include "App.h"

// \todo To be removed.
extern MIDIDevice midiDevice1;
extern MIDIDevice midiDevice2;
extern USBDriver *drivers[];
#define CNT_DEVICES 3
extern const char *driverNames[CNT_DEVICES];
extern bool driverActive[CNT_DEVICES];
extern USBDevice USBDevices[NR_OF_MIDI_INTERFACES];

/*
 * scan USB host for new devices
 */
void scanUSBHost(void)
{
    //logMessage ("scanUSBHost");

    usbHost.Task();

    App *app = App::get();

    for (uint8_t i = 0; i < CNT_DEVICES; i++) {
        if (*drivers[i] != driverActive[i]) {
            if (driverActive[i]) {
                driverActive[i] = false;

                logMessage("scanUSBHost: device %s %x:%x: disconnected",
                           driverNames[i],
                           drivers[i]->idVendor(),
                           drivers[i]->idProduct());
                for (uint8_t j = 0; j < NR_OF_MIDI_INTERFACES; j++) {
                    if (strcmp(driverNames[i], "MIDI1") == 0) {
                        USBDevices[0].reset();
                    } else {
                        USBDevices[1].reset();
                    }
                }
                app->statusBar.indicateUsbHostChange();
            } else {
                logMessage("scanUSBHost: device %s %x:%x: connected",
                           driverNames[i],
                           drivers[i]->idVendor(),
                           drivers[i]->idProduct());
                driverActive[i] = true;

                char *manufacturer = (char *)drivers[i]->manufacturer();

                if (manufacturer && *manufacturer) {
                    logMessage("- manufacturer: %s", manufacturer);
                }

                char *product = (char *)drivers[i]->product();
                if (product && *product) {
                    logMessage("- product: %s", product);
                }

                char *serial = (char *)drivers[i]->serialNumber();
                if (serial && *serial) {
                    logMessage("- serialNumber: %s", serial);
                }

                if (drivers[i] == &midiDevice1) {
                    logMessage("Assigning new MIDI1 device to midiBus 0: %s",
                               product);
                    USBDevices[0].set(drivers[i]->idVendor(),
                                      drivers[i]->idProduct(),
                                      driverNames[i],
                                      manufacturer,
                                      product,
                                      serial,
                                      0,
                                      &midiDevice1);
                    USBDevices[0].midiDevice->setHandleSysEx(
                        handleMidiUsbHostSysExPort0);
                }
                if (drivers[i] == &midiDevice2) {
                    logMessage("Assigning new MIDI2 device to midiBus 0: %s",
                               product);
                    USBDevices[1].set(drivers[i]->idVendor(),
                                      drivers[i]->idProduct(),
                                      driverNames[i],
                                      manufacturer,
                                      product,
                                      serial,
                                      1,
                                      &midiDevice2);
                    USBDevices[1].midiDevice->setHandleSysEx(
                        handleMidiUsbHostSysExPort1);
                }
            }

            app->statusBar.indicateUsbHostChange();
            System::windowManager.repaintActive();
        }
    }
}
