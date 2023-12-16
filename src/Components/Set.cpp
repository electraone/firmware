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

#include "Set.h"
#include "LookAndFeel.h"

Set::Set() : colour(Colours565::white), label(""), isHighligted(false)
{
}

Set::Set(Rectangle newBounds,
         uint32_t newColour,
         const char *newLabel,
         bool newIsHighligted)
    : colour(newColour), label(newLabel), isHighligted(newIsHighligted)
{
    setBounds(newBounds);
}

void Set::setColour(uint32_t newColour)
{
    colour = newColour;
    repaint();
}

void Set::setLabel(const char *newLabel)
{
    label = newLabel;
    repaint();
}

void Set::setHighlighted(bool shouldBeHighlighted)
{
    isHighligted = shouldBeHighlighted;
    repaint();
}

void Set::paint(Graphics &g)
{
    Rectangle bounds = getBounds();
    uint16_t width = bounds.getWidth();
    uint16_t height = bounds.getHeight();

    g.setColour(LookAndFeel::backgroundColour);
    g.fillRect(0, 0, width, 15);

    if (isHighligted) {
        paintHighligted(g, width, height, colour);
    } else {
        paintDefault(g, width, height, colour);
        g.paintTextPlaceHolder(
            0, 2, label, TextStyle::smallTransparent, width, TextAlign::center);
    }

    if (strlen(label) > 0) {
        g.printText(
            0, 2, label, TextStyle::smallTransparent, width, TextAlign::center);
    }
}

void Set::paintHighligted(Graphics &g,
                          uint16_t width,
                          uint16_t height,
                          uint32_t colour)
{
    if (height > 20) {
        g.setColour(Colours565::darker(colour, 0.2));
        g.drawRoundRect(0, 7, width, height - 7, 2);
    }
    g.setColour(Colours565::darker(colour, 0.25));
    g.fillRoundRect(0, 0, width, 15, 2);
}

void Set::paintDefault(Graphics &g,
                       uint16_t width,
                       uint16_t height,
                       uint32_t colour)
{
    g.setColour(Colours565::darker(colour, 0.2));
    if (height < 20) {
        g.drawLine(0, 7, width, 7);
    } else {
        g.drawRoundRect(0, 7, width, height - 7, 2);
    }
    //    g.setColour(Colours565::yellow);
    //    g.fillRect(50, 0, width/2, 15);
}
