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
 * @file TouchEvent.h
 *
 * @brief Attributes of a LCD touch event.
 */

#pragma once

#include "Touch.h"

class Component;

/**
 * @brief TouchEvent contains information about position and status of
 * LCD touch points.
 */
class TouchEvent : private TouchPoint
{
public:
    /**
     * Creates a TouchEvent instance.
     *
     * The TouchEvent is constructed by the Electra One firmware.
     *
     * @param touchPoint an output of the hardware touch driver
     */
    TouchEvent(const TouchPoint &touchPoint);

    /**
     * Returns the identifier of the touch point.
     *
     * @return identifier of the touch point.
     */
    uint8_t getId() const;

    /**
     * Returns type of the touch event.
     *
     * @return type of the touch event.
     */
    TouchPoint::Event getEvent() const;

    /**
     * Returns the X position of the touch point, using the screen coordinates.
     *
     * @return X position.
     */
    uint16_t getScreenX(void) const;

    /**
     * Returns the Y position of the touch point, using the screen coordinates.
     *
     * @return Y position.
     */
    uint16_t getScreenY(void) const;

    /**
     * Returns the X position of the touch point, using the relative coordinates
     *   within the component.
     *
     * @return X position.
     */
    uint16_t getX(void) const;

    /**
     * Returns the Y position of the touch point, using the relative coordinates
     *   within the component.
     *
     * @return Y position.
     */
    uint16_t getY(void) const;

    /**
     * Returns the X position of the initial touch point,
     * using the screen coordinates.
     *
     * @return X position.
     */
    uint16_t getTouchDownScreenX(void) const;

    /**
     * Returns the Y position of the initial touch point,
     * using the screen coordinates.
     *
     * @return Y position.
     */
    uint16_t getTouchDownScreenY(void) const;

    /**
     * Returns the X position of the initial touch point,
     * using the relative coordinates within the component.
     *
     * @return X position.
     */
    uint16_t getTouchDownX(void) const;

    /**
     * Returns the Y position of the initial touch point,
     * using the relative coordinates within the component.
     *
     * @return Y position.
     */
    uint16_t getTouchDownY(void) const;

    /**
     * Returns information if the touch point has been held down for longer
     * time period.
     *
     * @return true, on long touch.
     */
    bool isLongHold() const;

    /**
     * Returns pointer to a component that received the touch event.
     *
     * @return pointer to a component, or nullptr.
     */
    Component *getEventComponent(void) const;

    // Functions used by the Electra One firmware internally.

    /**
     * Sets a pointer to the component.
     *
     * @param a pointer to the component.
     */
    void setEventCompoment(Component *component);

private:
    Component *eventComponent;
};
