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
 * @file PotEvent.h
 *
 * @brief Attributes of a potentiometer event.
 */

#pragma once

class Component;

class PotEvent
{
public:
    PotEvent()
    {
    }

    PotEvent(uint8_t newPotId) : potId(newPotId), relativeChange(0)
    {
    }

    PotEvent(uint8_t newPotId, int16_t newRelativeChange)
        : potId(newPotId), relativeChange(newRelativeChange)
    {
    }

    uint8_t getPotId(void) const
    {
        return (potId);
    }

    int16_t getRelativeChange(void) const
    {
        return (relativeChange);
    }

    void setAcceleratedChange(int16_t newAcceleratedChange)
    {
        acceleratedChange = newAcceleratedChange;
    }

    int16_t getAcceleratedChange(void) const
    {
        return (acceleratedChange);
    }

    Component *getEventComponent(void) const
    {
        return (eventComponent);
    }

    void setEventCompoment(Component *component)
    {
        eventComponent = component;
    }

private:
    uint8_t potId;
    int16_t relativeChange;
    int16_t acceleratedChange;
    Component *eventComponent;
};
