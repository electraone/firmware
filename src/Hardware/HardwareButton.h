#pragma once

#include <functional>
#include <cstdint>

/**
 * Button class
 *
 * a representation of the hardware button.
 */
class HardwareButton
{
public:
    /**
     * List of possible hardware button events
     */
    enum Event {
        none, ///< no event
        press, ///< button has been pressed
        hold, ///< button has been held pressed for longer period
        release ///< button has been released
    };

    /**
     * A hardware button constructor
     *
     * @param id An numeric identifier of the button. The id is given by the
     *        hardware design.
     */
    HardwareButton(uint8_t id);

    /**
     * Test state of the button
     *
     * Evaluates the state of the button. If there is any change to the previous
     * state, the function will indicate it with the return value.
     *
     * @return true, if there is any state change.
     */
    Event process(void);

    /**
     * Get id of the button
     *
     * @return Identifier that was assigned when the button was created.
     */
    uint8_t getId(void);

    /**
     * Provide information if the button is pressed
     *
     * @return true, when button is currently pressed.
     */
    bool isPressed(void);

    /**
     * Provide information is the button is pressed for longer time period
     *
     * @return true, when pressed for longer time period.
     */
    bool isLongHold(void);

    /**
     * onPress event handler
     *
     * Callback function to handle event when the button is pressed down.
     */
    std::function<void(void)> onPress;

    /**
     * onLongHold event handler
     *
     * Callback function to handle event when the button is pressed for
     * longer time period.
     */
    std::function<void(void)> onLongHold;

    /**
     * onRelease event handler
     *
     * Callback function to handle event when the button is released.
     */
    std::function<void(void)> onRelease;

private:
    uint8_t id;
    bool pressed;
    uint8_t pinState;
    bool longHold;
    uint32_t pressStart;

    static constexpr uint16_t longHoldPeriod = 1000;
};
