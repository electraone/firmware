#include "TaskReadButtons.h"
#include "Hardware.h"
#include "ButtonListener.h"

/*
 * Read buttons
 */
void readButtons(void)
{
    HardwareButton *button = Hardware::buttons.getNext();

    switch (button->process()) {
        case HardwareButton::Event::press:
            ButtonListener::buttonBroadcaster.buttonDown(button->getId());
            break;

        case HardwareButton::Event::hold:
            ButtonListener::buttonBroadcaster.buttonLongHold(button->getId());
            break;

        case HardwareButton::Event::release:
            ButtonListener::buttonBroadcaster.buttonUp(button->getId());
            break;

        default:
            // Ignored on purpose
            break;
    }
}
