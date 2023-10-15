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
 * @file List.cpp
 *
 * @brief A Text / Bitmap Selection List component.
 */

#include "List.h"

List::List(const ListData *newListData) : AssignableList(newListData), index(0)
{
}

void List::setColour(uint32_t newColour)
{
    colour = newColour;
    repaint();
}

void List::setIndex(uint16_t newIndex)
{
    index = constrain(newIndex, 0, getList()->getMaxIndex());
    repaint();
}

uint16_t List::getIndex(void) const
{
    return (index);
}

void List::onTouchMove([[maybe_unused]] const TouchEvent &touchEvent)
{
}

void List::onTouchDown([[maybe_unused]] const TouchEvent &touchEvent)
{
}

void List::onTouchUp([[maybe_unused]] const TouchEvent &touchEvent)
{
}

void List::onPotChange(const PotEvent &potEvent)
{
    setIndex(std::max(0, index + potEvent.getAcceleratedChange()));
}

void List::onPotTouchDown([[maybe_unused]] const PotEvent &potEvent)
{
}

void List::onPotTouchUp([[maybe_unused]] const PotEvent &potEvent)
{
}

void List::paint(Graphics &g)
{
    g.fillAll(LookAndFeel::backgroundColour);
    LookAndFeel::paintList(g, getBounds(), colour, getList(), index);
}

void List::resized(void)
{
    repaint();
}
