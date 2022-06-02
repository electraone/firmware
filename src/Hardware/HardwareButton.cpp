#include "core_pins.h"
#include "helpers.h"
#include "HardwareButton.h"
#include "Mux.h"

HardwareButton::HardwareButton(uint8_t newId)
    : id(newId), pressed(false), pinState(1), longHold(false), pressStart(0)
{
}

HardwareButton::Event HardwareButton::process(void)
{
    Event eventDetected = none;

    setMuxAddress(id);
    MUX_ENABLE;
    pinState = digitalReadFast(41);

    if ((pinState == 0) && (pressed == false)) {
        eventDetected = press;
        pressed = true;
        longHold = false;
        pressStart = millis();

        if (onPress) {
            onPress();
        }
    } else if (pinState == 0) {
        if (((millis() - pressStart) > longHoldPeriod) && (longHold == false)) {
            eventDetected = hold;
            longHold = true;

            if (onLongHold) {
                onLongHold();
            }
        }
    } else if ((pinState == 1) && (pressed == true)) {
        pressed = false;
        pressStart = 0;
        eventDetected = release;

        if (onRelease) {
            onRelease();
        }
    }
    MUX_DISABLE;

    return (eventDetected);
}

uint8_t HardwareButton::getId(void)
{
    return (id);
}

bool HardwareButton::isPressed(void)
{
    return (pressed);
}

bool HardwareButton::isLongHold(void)
{
    return (longHold);
}
