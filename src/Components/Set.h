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
 * @file Set.h
 *
 * @brief A component for grouping components to sets.
 */

#pragma once

#include "Component.h"

class Set : virtual public Component
{
public:
    Set();
    Set(Rectangle newBounds,
        uint32_t newColour,
        const char *newLabel,
        bool newIsHighligted);
    virtual ~Set() = default;

    void setColour(uint32_t newColour);
    void setLabel(const char *newLabel);
    void setHighlighted(bool shouldBeHighlighted);
    void paint(Graphics &g);

private:
    static void paintHighligted(Graphics &g,
                                uint16_t width,
                                uint16_t height,
                                uint32_t colour);
    static void paintDefault(Graphics &g,
                             uint16_t width,
                             uint16_t height,
                             uint32_t colour);

    static constexpr uint8_t MaxSetLabelLength = 40;
    uint32_t colour;
    const char *label;
    bool isHighligted;
};
