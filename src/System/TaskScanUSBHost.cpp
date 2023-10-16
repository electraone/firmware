/*
* Electra One MIDI Controller Firmware
* See COPYRIGHT file at the top of the source tree.
*
* This product includes software developed by the
* Electra One Project (http://electra.one/).
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.
*/

#include "TaskScanUSBHost.h"
#include "SysexCallbacks.h"
#include "System.h"
#include "MidiOutput.h"
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
    usbHost.Task();

    App *app = App::get();

    for (uint8_t i = 0; i < CNT_DEVICES; i++) {
        if (*drivers[i] != driverActive[i]) {
            if (driverActive[i]) {
                driverActive[i] = false;

                System::logger.write(
                    LOG_ERROR,
                    "scanUSBHost: device %s %x:%x: disconnected",
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
                System::logger.write(LOG_ERROR,
                                     "scanUSBHost: device %s %x:%x: connected",
                                     driverNames[i],
                                     drivers[i]->idVendor(),
                                     drivers[i]->idProduct());
                driverActive[i] = true;

                char *manufacturer = (char *)drivers[i]->manufacturer();

                if (manufacturer && *manufacturer) {
                    System::logger.write(
                        LOG_ERROR, "- manufacturer: %s", manufacturer);
                }

                char *product = (char *)drivers[i]->product();
                if (product && *product) {
                    System::logger.write(LOG_ERROR, "- product: %s", product);
                }

                char *serial = (char *)drivers[i]->serialNumber();
                if (serial && *serial) {
                    System::logger.write(
                        LOG_ERROR, "- serialNumber: %s", serial);
                }

                uint8_t port = App::get()->getUsbHostPortAssigment(product);

                if (drivers[i] == &midiDevice1) {
                    System::logger.write(
                        LOG_ERROR,
                        "Assigning new MIDI1 device to midiBus 0: %s",
                        product);
                    USBDevices[0].set(drivers[i]->idVendor(),
                                      drivers[i]->idProduct(),
                                      driverNames[i],
                                      manufacturer,
                                      product,
                                      serial,
                                      port,
                                      &midiDevice1);
                    USBDevices[0].midiDevice->setHandleSysEx(
                        handleMidiUsbHostSysExPort0);
                }
                if (drivers[i] == &midiDevice2) {
                    System::logger.write(
                        LOG_ERROR,
                        "Assigning new MIDI2 device to midiBus 0: %s",
                        product);
                    USBDevices[1].set(drivers[i]->idVendor(),
                                      drivers[i]->idProduct(),
                                      driverNames[i],
                                      manufacturer,
                                      product,
                                      serial,
                                      port,
                                      &midiDevice2);
                    USBDevices[1].midiDevice->setHandleSysEx(
                        handleMidiUsbHostSysExPort1);
                }

                if ((drivers[i]->idVendor() == 0x1FC9)
                    && (drivers[i]->idProduct() == 0x82CF)) {
                    System::logger.write(LOG_ERROR, "electra is connected");
                }
            }

            app->statusBar.indicateUsbHostChange();
            System::windowManager.repaintActive();
        }
    }
}
