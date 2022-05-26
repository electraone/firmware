#include "ButtonBroadcaster.h"
#include "ButtonListener.h"
#include "helpers.h"

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
    auto i = listeners.begin();

    while (i != listeners.end()) {
        auto current = i++;
        auto listener = current->first;

        if (listeners[listener] == 1) {
            if ((listener->buttonId == ButtonListener::AllButtons)
                || (buttonId == listener->buttonId)) {
                (listener->*function)(buttonId);
            }
        }
    }
}

void ButtonBroadcaster::listListeners(void)
{
    for (auto i = listeners.begin(); i != listeners.end(); i++) {
        auto listener = i->first;
        logMessage("listListeners: buttonListener: %x is suspended: %d",
                   listener,
                   listeners[listener]);
    }
}
