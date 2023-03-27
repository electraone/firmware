#include "Hardware.h"
#include "System.h"
#include "Reset.h"

void Hardware::initialise(void)
{
    // Configure and initialise internal signal multiplexer
    configureMux();
    setMuxAddress(0);
    MUX_DISABLE;
    System::logger.write(LOG_ERROR, "Multiplexer: initialised");

    // Configure AD convertors
    adc.adc0->setAveraging(8);
    adc.adc1->setAveraging(8);
    adc.adc0->setResolution(10);
    adc.adc1->setResolution(10);
    System::logger.write(LOG_ERROR, "AD convertor: initialised");

    // Initialise RAM monitoring
    ram.initialize();
    System::logger.write(LOG_ERROR, "SD RAM monitor: initialised");

    // Clear Serial ports
    Serial1.clear();
    Serial2.clear();

    // Initialise the LCD
    screen.initialise();
    System::logger.write(LOG_ERROR, "LCD: initialised");

    // Initialise LCD touch interface
    touch.begin();
    System::logger.write(LOG_ERROR, "LCD touch: initialided");

    // Initialise pots
    pots.initialise();
    System::logger.write(LOG_ERROR, "Pots: initialised");

    // Mount the internal storage media
    Hardware::sdcard.mount();

    // Assign the storage drivers.
    memory.stringPool.assignStorageDriver(&Hardware::screen);
    memory.bitmapPool.assignStorageDriver(&Hardware::screen);
}

void Hardware::reset(void)
{
    CPU_RESTART;
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
