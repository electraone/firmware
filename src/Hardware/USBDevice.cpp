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
