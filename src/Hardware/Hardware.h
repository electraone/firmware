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

class Hardware
{
public:
    Hardware() = delete;

    static void initialise(void);

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
