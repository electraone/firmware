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
 * @file MeterHorizontal.h
 *
 * @brief A Horizontal style Meter component.
 */

#pragma once

#include "LookAndFeel.h"
#include "Slider.h"

class MeterHorizontal : public Slider
{
public:
    MeterHorizontal();
    virtual ~MeterHorizontal() override = default;

    void setColourInactive(uint32_t newColour);
    void setColourBackground(uint32_t newColour);

    virtual void onTouchMove(const TouchEvent &touchEvent) override;
    virtual void onTouchDown(const TouchEvent &touchEvent) override;
    virtual void onTouchUp(const TouchEvent &touchEvent) override;

    void paint(Graphics &g) override;

private:
    uint32_t colourInactive;
    uint32_t colourBackground;
};
