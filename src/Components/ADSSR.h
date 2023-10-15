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
 * @file ADSSR.h
 *
 * @brief An ADSSR envelope component.
 */

#pragma once

#include "Envelope.h"

class ADSSR : public Envelope
{
public:
    ADSSR()
    {
        points.push_back(Point()); // Attack
        points.push_back(Point()); // Decay
        points.push_back(Point()); // Break / Slope
        points.push_back(Point()); // Sustain
        points.push_back(Point()); // Release

        values.push_back(Value()); // Attack
        values.push_back(Value()); // Decay
        values.push_back(Value()); // Break point
        values.push_back(Value()); // Slope
        values.push_back(Value()); // Sustain
        values.push_back(Value()); // Release
    }

    virtual ~ADSSR() = default;

    void computePoints(const Rectangle &bounds)
    {
        uint16_t segmentWidth = getSegmentWidth(bounds.getWidth(), 5);
        float maxY = bounds.getHeight() - 1;

        // Find the lowest point of the envelope
        float lowestY =
            std::min(values[breakPoint].get(), values[sustain].get());

        // Set the baseline
        baselineY = map(
            0.0f, std::min(0.0f, lowestY), values[sustain].get(), maxY, 0.0f);

        int16_t sustainLevel = map(values[sustain].get(),
                                   std::min(0.0f, values[sustain].get()),
                                   values[sustain].getMax(),
                                   maxY,
                                   0.0f);

        int16_t breakLevel = map(values[breakPoint].get(),
                                 std::min(0.0f, values[breakPoint].get()),
                                 values[breakPoint].getMax(),
                                 maxY,
                                 0.0f);

        // Starting point
        points[0].x = 0;
        points[0].y = baselineY;

        // Delay
        points[1].x = segmentWidth * 0; // values[delay].getRelative();
        points[1].y = baselineY;

        // Attack
        points[2].x = points[1].x + segmentWidth * values[attack].getRelative();
        points[2].y = 0;

        // Decay
        points[3].x = points[2].x + segmentWidth * values[decay].getRelative();
        points[3].y = breakLevel;

        // Slope
        points[4].x = points[3].x + segmentWidth * values[slope].getRelative();
        points[4].y = sustainLevel;

        // Sustain
        points[5].x = points[4].x + segmentWidth;
        points[5].y = points[4].y;

        // Release
        points[6].x =
            points[5].x + segmentWidth * values[release].getRelative();
        points[6].y = baselineY;
    }

    static constexpr uint8_t attack = 0;
    static constexpr uint8_t decay = 1;
    static constexpr uint8_t breakPoint = 2;
    static constexpr uint8_t slope = 3;
    static constexpr uint8_t sustain = 4;
    static constexpr uint8_t release = 5;

private:
};
