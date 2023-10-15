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
 * @file SliderHorizontal.cpp
 *
 * @brief A Horizontal Slider component.
 */

#include "SliderHorizontal.h"

SliderHorizontal::SliderHorizontal()
{
}

void SliderHorizontal::onTouchMove(const TouchEvent &touchEvent)
{
    int16_t max = value.getMax();
    int16_t min = value.getMin();

    float step = getWidth() / (float)(max - min);
    int16_t newValue =
        constrain(ceil(touchEvent.getX() / step + min), min, max);

    setValue(newValue);

    if (onValueChange) {
        onValueChange(value.get());
    }
}

void SliderHorizontal::onTouchDown(
    [[maybe_unused]] const TouchEvent &touchEvent)
{
}

void SliderHorizontal::onTouchUp([[maybe_unused]] const TouchEvent &touchEvent)
{
}

void SliderHorizontal::paint(Graphics &g)
{
    g.fillAll(LookAndFeel::backgroundColour);
    LookAndFeel::paintSliderHorizontal(
        g, getBounds(), colour, value.getMin(), value.getMax(), value.get());
}
