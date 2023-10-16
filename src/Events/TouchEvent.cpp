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
 * @file TouchEvent.cpp
 *
 * @brief Attributes of a LCD touch event.
 */

#include "TouchEvent.h"
#include "Component.h"

TouchEvent::TouchEvent(const TouchPoint &touchPoint)
    : TouchPoint(touchPoint), eventComponent(nullptr)
{
}

uint8_t TouchEvent::getId() const
{
    return (id);
}

TouchPoint::Event TouchEvent::getEvent() const
{
    return (event);
}

uint16_t TouchEvent::getScreenX(void) const
{
    return (x);
}

uint16_t TouchEvent::getScreenY(void) const
{
    return (y);
}

uint16_t TouchEvent::getX(void) const
{
    if (eventComponent) {
        return (constrain(
            x - eventComponent->getScreenX(), 0, eventComponent->getWidth()));
    }

    return x;
}

uint16_t TouchEvent::getY(void) const
{
    if (eventComponent) {
        return (constrain(
            y - eventComponent->getScreenY(), 0, eventComponent->getHeight()));
    }

    return y;
}

uint16_t TouchEvent::getTouchDownScreenX(void) const
{
    return (xStart);
}

uint16_t TouchEvent::getTouchDownScreenY(void) const
{
    return (yStart);
}

uint16_t TouchEvent::getTouchDownX(void) const
{
    if (eventComponent) {
        return (constrain(xStart - eventComponent->getScreenX(),
                          0,
                          eventComponent->getWidth()));
    }

    return x;
}

uint16_t TouchEvent::getTouchDownY(void) const
{
    if (eventComponent) {
        return (constrain(yStart - eventComponent->getScreenY(),
                          0,
                          eventComponent->getHeight()));
    }

    return y;
}

Component *TouchEvent::getEventComponent(void) const
{
    return (eventComponent);
}

void TouchEvent::setEventCompoment(Component *component)
{
    eventComponent = component;
}
