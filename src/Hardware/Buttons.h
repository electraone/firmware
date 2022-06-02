#pragma once

#include "settings.h"
#include "HardwareButton.h"

// \todo replace macros with enum
#define BUTTON_LEFT_TOP 0
#define BUTTON_LEFT_MIDDLE 1
#define BUTTON_LEFT_BOTTOM 2
#define BUTTON_RIGHT_TOP 3
#define BUTTON_RIGHT_MIDDLE 4
#define BUTTON_RIGHT_BOTTOM 5

/**
 * Buttons class
 *
 * The class holds an array of hardware buttons. Their ids and their position
 * in the private buttons[] array matters and must reflect hardware layout
 */
class Buttons
{
public:
    HardwareButton *operator[](const uint8_t id);
    HardwareButton *getNext(void);

private:
    HardwareButton buttons[NR_OF_HW_BUTTONS] = {
        HardwareButton(BUTTON_LEFT_TOP),     HardwareButton(BUTTON_LEFT_MIDDLE),
        HardwareButton(BUTTON_LEFT_BOTTOM),  HardwareButton(BUTTON_RIGHT_TOP),
        HardwareButton(BUTTON_RIGHT_MIDDLE), HardwareButton(BUTTON_RIGHT_BOTTOM)
    };
    uint8_t currentId = 0;
};
