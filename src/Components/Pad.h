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
 * @file Pad.h
 *
 * @brief A Pad component.
 */

#pragma once

#include "Component.h"
#include "LookAndFeel.h"

class Pad : virtual public Component
{
public:
    Pad();
    virtual ~Pad() = default;
    void setColour(uint32_t newColour);
    void setState(bool newState);
    bool getState(void) const;
    bool isDown(void) const;

    virtual void onTouchMove(const TouchEvent &touchEvent) override;
    virtual void onTouchDown(const TouchEvent &touchEvent) override;
    virtual void onTouchUp(const TouchEvent &touchEvent) override;

    virtual void onPotChange(const PotEvent &potEvent) override;
    virtual void onPotTouchDown(const PotEvent &potEvent) override;
    virtual void onPotTouchUp(const PotEvent &potEvent) override;

    virtual void resized(void) override;
    virtual void paint(Graphics &g) override;

protected:
    uint32_t colour;
    bool state;
};
