#include "ButtonBroadcaster.h"
#include "ButtonListener.h"

void ButtonBroadcaster::buttonDown(uint8_t buttonId)
{
    triggerCallbacks(buttonId, &ButtonListener::onButtonDown);
}

void ButtonBroadcaster::buttonUp(uint8_t buttonId)
{
    triggerCallbacks(buttonId, &ButtonListener::onButtonUp);
}

void ButtonBroadcaster::buttonLongHold(uint8_t buttonId)
{
    triggerCallbacks(buttonId, &ButtonListener::onButtonLongHold);
}

void ButtonBroadcaster::triggerCallbacks(uint8_t buttonId, blFunction function)
{
    for (auto i = listeners.begin(); i != listeners.end(); i++) {
        auto listener = i->first;

        if ((listener->buttonId == ButtonListener::AllButtons)
            || (buttonId == listener->buttonId)) {
            (listener->*function)(buttonId);
        }
    }
}
