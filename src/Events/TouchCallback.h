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
 * @file TouchCallback.h
 *
 * @brief A callback handler for LCD touch events.
 */

#pragma once

#include "TouchEvent.h"

/**
 * @brief provides callbacks for handing LCD touch events.
 */
class TouchCallback
{
public:
    /**
     * Destructor
     */
    virtual ~TouchCallback() = default;

    /**
     * Called when a finger makes in the inital touch on the LCD.
     *
     * @param details about the position and status of the touch event,
     *        including the component in which it occurred.
     */
    virtual void onTouchDown(const TouchEvent &touchEvent)
    {
    }

    /**
     * Called when a finger is lifted off the LCD.
     *
     * @param details about the position and status of the touch event,
     *        including the component in which it occurred.
     */
    virtual void onTouchUp(const TouchEvent &touchEvent)
    {
    }

    /**
     * Called when there is a short single tap on the LCD.
     *
     * @param details about the position and status of the touch event,
     *        including the component in which it occurred.
     */
    virtual void onTouchClick(const TouchEvent &touchEvent)
    {
    }

    /**
     * Called when there is a double tap on the LCD.
     *
     * @param details about the position and status of the touch event,
     *        including the component in which it occurred.
     */
    virtual void onTouchDoubleClick(const TouchEvent &touchEvent)
    {
    }

    /**
     * Called when the finger touches the LCD and moves its position.
     *
     * @param details about the position and status of the touch event,
     *        including the component in which it occurred.
     */
    virtual void onTouchMove(const TouchEvent &touchEvent)
    {
    }

    /**
     * Called when the finger si touching the LCD for longer time period.
     *
     * It does not matter if the finger is held at the same place or if it
     * is moving.
     *
     * @param details about the position and status of the touch event,
     *        including the component in which it occurred.
     */
    virtual void onTouchLongHold(const TouchEvent &touchEvent)
    {
    }
};
