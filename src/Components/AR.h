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
 * @file AR.h
 *
 * @brief An AR envelope component.
 */

#pragma once

#include "Envelope.h"

class AR : public Envelope
{
public:
    AR()
    {
        points.push_back(Point()); // Attack
        points.push_back(Point()); // Release (or Decay)

        values.push_back(Value()); // Attack
        values.push_back(Value()); // Release (or Decay)
    }

    virtual ~AR() = default;

    void computePoints(const Rectangle &bounds)
    {
        uint16_t segmentWidth = getSegmentWidth(bounds.getWidth(), 2);

        // Set the baseline
        baselineY = bounds.getHeight() - 1;

        // Starting point
        points[0].x = 0;
        points[0].y = baselineY;

        // Delay
        points[1].x = segmentWidth * 0; // values[delay].getRelative();
        points[1].y = baselineY;

        // Attack
        points[2].x = points[1].x + segmentWidth * values[attack].getRelative();
        points[2].y = 0;

        // Release
        points[3].x =
            points[2].x + segmentWidth * values[release].getRelative();
        points[3].y = baselineY;
    }

    static constexpr uint8_t attack = 0;
    static constexpr uint8_t release = 1;

private:
};
