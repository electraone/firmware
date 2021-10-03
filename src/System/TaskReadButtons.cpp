#include "TaskReadButtons.h"
#include "Hardware.h"
#include "ButtonListener.h"

/*
 * Read buttons
 */
void readButtons(void)
{
    Button *button = Hardware::buttons.getNext();

    switch (button->process()) {
        case Button::Event::press:
            ButtonListener::buttonBroadcaster.buttonDown(button->getId());
            break;

        case Button::Event::hold:
            ButtonListener::buttonBroadcaster.buttonLongHold(button->getId());
            break;

        case Button::Event::release:
            ButtonListener::buttonBroadcaster.buttonUp(button->getId());
            break;

        default:
            // Ignored on purpose
            break;
    }
}
