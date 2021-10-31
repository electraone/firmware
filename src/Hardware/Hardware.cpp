#include "Hardware.h"
#include "helpers.h"

/* This is to be able to initiate USB stack in here. To give more
 * control over the USB device configuration
 */
extern "C" void usb_init(void);

void Hardware::initialise(void)
{
    // Configure and initialise internal signal multiplexer
    configureMux();
    setMuxAddress(0);
    MUX_DISABLE;
    logMessage("Multiplexer: initialised");

    // Configure AD convertors
    adc.adc0->setAveraging(8);
    adc.adc1->setAveraging(8);
    adc.adc0->setResolution(10);
    adc.adc1->setResolution(10);
    logMessage("AD convertor: initialised");

    // Initialise pots
    pots.initialise();
    logMessage("Pots: initialised");

    // Initialise LCD touch interface
    touch.begin();
    logMessage("LCD touch: initialided");

    // Initialise RAM monitoring
    ram.initialize();
    logMessage("SD RAM monitor: initialised");

    // initialize USB device.
    usb_init();
    logMessage("USB device ports: initialised");

    // Clear Serial ports
    Serial1.clear();
    Serial2.clear();

    // Initialise the LCD
    screen.initialise();
    logMessage("LCD: initialised");

    // Mount the internal storage media
    Hardware::sdcard.mount();

    // Assign the storage drivers.
    memory.stringPool.assignStorageDriver(&Hardware::screen);
    memory.bitmapPool.assignStorageDriver(&Hardware::screen);
}

Pots Hardware::pots;
PotTouch Hardware::potTouch;
Buttons Hardware::buttons;
ADC Hardware::adc;
Touch Hardware::touch;
Graphics Hardware::screen(RA8876_CS, RA8876_RESET);
PersistentStorage Hardware::sdcard;
RamMonitor Hardware::ram;
Memory Hardware::memory;
