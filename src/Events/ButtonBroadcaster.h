#pragma once

#include "Broadcaster.h"

class ButtonListener;

typedef void (ButtonListener::*blFunction)(uint8_t);

class ButtonBroadcaster : public Broadcaster<ButtonListener>
{
public:
    //using Broadcaster<ButtonListener>::Broadcaster;

    void buttonDown(uint8_t buttonId);
    void buttonUp(uint8_t buttonId);
    void buttonLongHold(uint8_t buttonId);

    void listListeners(void);

private:
    void triggerCallbacks(uint8_t buttonId, blFunction function);
};
