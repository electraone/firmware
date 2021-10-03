#pragma once

#include "settings.h"
#include "Button.h"

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
    Button *operator[](const uint8_t id);
    Button *getNext(void);

private:
    Button buttons[NR_OF_HW_BUTTONS] = {
        Button(BUTTON_LEFT_TOP),     Button(BUTTON_LEFT_MIDDLE),
        Button(BUTTON_LEFT_BOTTOM),  Button(BUTTON_RIGHT_TOP),
        Button(BUTTON_RIGHT_MIDDLE), Button(BUTTON_RIGHT_BOTTOM)
    };
    uint8_t currentId = 0;
};
