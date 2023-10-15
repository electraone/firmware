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
 * @file Pad.cpp
 *
 * @brief A Pad component.
 */

#include "Pad.h"

Pad::Pad() : state(false)
{
}

void Pad::setColour(uint32_t newColour)
{
    colour = newColour;
    repaint();
}

void Pad::setState(bool newState)
{
    state = newState;
    repaint();
}

bool Pad::getState(void) const
{
    return (state);
}

bool Pad::isDown(void) const
{
    return (state == true);
}

void Pad::onTouchMove([[maybe_unused]] const TouchEvent &touchEvent)
{
}

void Pad::onTouchDown([[maybe_unused]] const TouchEvent &touchEvent)
{
    state = !state;
    repaint();
}

void Pad::onTouchUp([[maybe_unused]] const TouchEvent &touchEvent)
{
}

void Pad::onPotChange([[maybe_unused]] const PotEvent &potEvent)
{
}

void Pad::onPotTouchDown([[maybe_unused]] const PotEvent &potEvent)
{
}

void Pad::onPotTouchUp([[maybe_unused]] const PotEvent &potEvent)
{
}

void Pad::paint(Graphics &g)
{
    LookAndFeel::paintPad(g, getBounds(), colour, false, state);
}

void Pad::resized(void)
{
    repaint();
}
