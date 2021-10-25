#pragma once

#include "Envelope.h"

class ADR : public Envelope
{
public:
    ADR()
    {
        points.push_back(Point()); // Attack
        points.push_back(Point()); // Decay
        points.push_back(Point()); // Release

        values.push_back(Value()); // Attack
        values.push_back(Value()); // Decay
        values.push_back(Value()); // Release
    }

    virtual ~ADR() = default;

    void computePoints(const Rectangle &bounds)
    {
        uint16_t segmentWidth = getSegmentWidth(bounds.getWidth(), 3);
        uint16_t decayLength = segmentWidth * values[decay].getRelative();
        float factor = (float)decayLength / (float)segmentWidth;

        // Set the baseline
        baselineY = bounds.getHeight() - 1;

        // Starting point
        points[0].x = 0;
        points[0].y = baselineY;

        // Delay
        points[1].x = segmentWidth * values[delay].getRelative();
        points[1].y = baselineY;

        // Attack
        points[2].x = points[1].x + segmentWidth * values[attack].getRelative();
        points[2].y = 0;

        // Decay
        points[3].x = points[2].x + decayLength;
        points[3].y = baselineY - (baselineY * factor);

        // Release
        points[4].x =
            points[3].x + segmentWidth * values[release].getRelative();
        points[4].y = points[0].y;
    }

    static constexpr uint8_t attack = 1;
    static constexpr uint8_t decay = 2;
    static constexpr uint8_t release = 3;

private:
};
