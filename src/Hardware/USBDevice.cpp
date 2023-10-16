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

#include "USBDevice.h"
#include "helpers.h"

/*
 * USB Device constructor
 */
USBDevice::USBDevice(uint8_t newManufacturerId,
                     uint8_t newProductId,
                     const char *newDriver,
                     const char *newManufacturer,
                     const char *newProduct,
                     const char *newSerial,
                     uint8_t newMidiBus,
                     MIDIDevice *newMidiDevice)
{
    set(newManufacturerId,
        newProductId,
        newDriver,
        newManufacturer,
        newProduct,
        newSerial,
        newMidiBus,
        newMidiDevice);
}

void USBDevice::set(uint8_t newManufacturerId,
                    uint8_t newProductId,
                    const char *newDriver,
                    const char *newManufacturer,
                    const char *newProduct,
                    const char *newSerial,
                    uint8_t newMidiBus,
                    MIDIDevice *newMidiDevice)
{
    manufacturerId = newManufacturerId;
    productId = newProductId;
    copyString(driver, newDriver, MAX_DRIVER_NAME_LENGTH);
    copyString(manufacturer, newManufacturer, MAX_MANUFACTURER_LENGTH);
    copyString(product, newProduct, MAX_PRODUCT_LENGTH);
    copyString(serial, newSerial, MAX_SERIAL_NUMBER_LENGTH);
    midiBus = newMidiBus;
    midiDevice = newMidiDevice;
}

void USBDevice::reset(void)
{
    set(0, 0, "NONE", "", "", "", 0, NULL);
}

void USBDevice::setMidiBus(uint8_t newMidiBus)
{
    midiBus = newMidiBus;
}
