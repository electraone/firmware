#pragma once

#include "Mux.h"
#include "Pots.h"
#include "PotTouch.h"
#include "Buttons.h"
#include "Touch.h"
#include "Graphics.h"
#include "PersistentStorage.h"
#include "Memory.h"
#include "RamMonitor.h"

// A list of all hardware features of Electra One platform.
// Members are made public on purpose.

/* This is to be able to initiate USB stack in here. To give more
 * control over the USB device configuration
 */
extern "C" void usb_init(uint8_t keyboardEnabled,
                         uint8_t mouseEnabled,
                         uint8_t keymediaEnabled);
extern "C" void usb_stop(void);


class Hardware
{
public:
    Hardware() = delete;

    static void initialise(void);
    static void reset(void);

    static Pots pots;
    static PotTouch potTouch;
    static Buttons buttons;
    static ADC adc;
    static Touch touch;
    static Graphics screen;
    static PersistentStorage sdcard;
    static Memory memory;
    static RamMonitor ram;
};
