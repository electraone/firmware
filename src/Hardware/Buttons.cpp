#include "Buttons.h"

HardwareButton *Buttons::operator[](const uint8_t id)
{
    return (&buttons[id]);
}

HardwareButton *Buttons::getNext(void)
{
    HardwareButton *button = &buttons[currentId++];

    if (currentId >= NR_OF_HW_BUTTONS) {
        currentId = 0;
    }

    return (button);
}
