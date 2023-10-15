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
 * @file MeterHorizontal.cpp
 *
 * @brief A Horizontal style Meter component.
 */

#include "MeterHorizontal.h"

MeterHorizontal::MeterHorizontal()
    : colourInactive(Colours565::grey),
      colourBackground(LookAndFeel::backgroundColour)
{
}

void MeterHorizontal::setColourInactive(uint32_t newColour)
{
    colourInactive = newColour;
    repaint();
}

void MeterHorizontal::setColourBackground(uint32_t newColour)
{
    colourBackground = newColour;
    repaint();
}

void MeterHorizontal::onTouchMove([[maybe_unused]] const TouchEvent &touchEvent)
{
}

void MeterHorizontal::onTouchDown([[maybe_unused]] const TouchEvent &touchEvent)
{
}

void MeterHorizontal::onTouchUp([[maybe_unused]] const TouchEvent &touchEvent)
{
}

void MeterHorizontal::paint(Graphics &g)
{
    g.fillAll(LookAndFeel::backgroundColour);
    LookAndFeel::paintMeterHorizontal(g,
                                      getBounds(),
                                      colour,
                                      colourInactive,
                                      colourBackground,
                                      value.getMin(),
                                      value.getMax(),
                                      value.get());
}
