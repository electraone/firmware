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

/**
 * @file UsbDevice.h
 *
 * @brief A class to hold info about connected USB device.
 */

#pragma once

#include <USBHost_t36.h>

#define MAX_DRIVER_NAME_LENGTH 10
#define MAX_MANUFACTURER_LENGTH 32
#define MAX_PRODUCT_LENGTH 32
#define MAX_SERIAL_NUMBER_LENGTH 32

/*
 * USB Device class
 *
 */
class USBDevice
{
public:
    USBDevice(uint8_t newManufacturerId = 0,
              uint8_t newProductId = 0,
              const char *newDriver = "",
              const char *newManufacturer = "",
              const char *newProduct = "",
              const char *newSerial = "",
              uint8_t newMidiBus = 0,
              MIDIDevice *newMidiDevice = nullptr);

    void set(uint8_t newManufacturerId,
             uint8_t newProductId,
             const char *newDriver,
             const char *newManufacturer,
             const char *newProduct,
             const char *newSerial,
             uint8_t newMidiBus,
             MIDIDevice *newMidiDevice);
    void reset(void);
    void setMidiBus(uint8_t newMidiBus);

    uint8_t manufacturerId;
    uint8_t productId;
    char driver[MAX_DRIVER_NAME_LENGTH + 1];
    char manufacturer[MAX_MANUFACTURER_LENGTH + 1];
    char product[MAX_PRODUCT_LENGTH + 1];
    char serial[MAX_SERIAL_NUMBER_LENGTH + 1];

    uint8_t midiBus;
    MIDIDevice *midiDevice;
};
