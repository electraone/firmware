#include "Buttons.h"

Button *Buttons::operator[](const uint8_t id)
{
    return (&buttons[id]);
}

Button *Buttons::getNext(void)
{
    Button *button = &buttons[currentId++];

    if (currentId >= NR_OF_HW_BUTTONS) {
        currentId = 0;
    }

    return (button);
}
