/*
* Electra One MIDI Controller Firmware
* See COPYRIGHT file at the top of the source tree.
*
* This product includes software developed by the
* Electra One Project (http://electra.one/).
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.
*/

/**
 * @file ButtonListener.h
 *
 * @brief A listener of hardware button events.
 */

#pragma once

#include <cstdint>
#include "ButtonBroadcaster.h"

/**
 * @brief provides callbacks for handling hardware button events.
 */
class ButtonListener
{
public:
    /**
     * Constructor.
     */
    ButtonListener() = default;

    /**
     * Destructor.
     */
    virtual ~ButtonListener() = default;

    /**
     * Called when the button is pressed.
     *
     * @param buttonId an identifier of the button.
     */
    virtual void onButtonDown(uint8_t buttonId)
    {
    }

    /**
     * Called when the button is released.
     *
     * @param buttonId an identifier of the button.
     */
    virtual void onButtonUp(uint8_t buttonId)
    {
    }

    /**
     * Called when the button is held down for longer time period.
     *
     * @param buttonId an identifier of the button.
     */
    virtual void onButtonLongHold(uint8_t buttonId)
    {
    }

    /**
     * Assigns the button to the listener object.
     *
     * @param an identifier of the button. It is a number in range 0 to 5, or
     *        AllButtons constant for catching events from all buttons.
     */
    void assignButton(uint8_t newButtonId = AllButtons)
    {
        buttonId = newButtonId;
        buttonBroadcaster.addListener(this);
    }

    /**
     * Assigns all buttons to the listener object.
     */
    void assignAllButtons(void)
    {
        buttonId = AllButtons;
        buttonBroadcaster.addListener(this);
    }

    /**
     * Stops listening for assigned buttons.
     */
    void releaseButton(void)
    {
        buttonBroadcaster.removeListener(this);
    }

    static constexpr uint8_t AllButtons = 255;
    static ButtonBroadcaster buttonBroadcaster;
    uint8_t buttonId;
};
