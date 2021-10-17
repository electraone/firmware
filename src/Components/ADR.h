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

    void computePoints(void)
    {
        uint16_t segmentWidth = getSegmentWidth(3);
        uint16_t decayLength = segmentWidth * values[decay].value;
        float factor = (float)decayLength / (float)segmentWidth;

        // Set the baseline
        baselineY = getHeight() - 1;

        // Starting point
        points[0].x = 0;
        points[0].y = baselineY;

        // Delay
        points[1].x = segmentWidth * values[delay].value;
        points[1].y = baselineY;

        // Attack
        points[2].x = points[1].x + segmentWidth * values[attack].value;
        points[2].y = 0;

        // Decay
        points[3].x = points[2].x + decayLength;
        points[3].y = baselineY - (baselineY * factor);

        // Release
        points[4].x = points[3].x + segmentWidth * values[release].value;
        points[4].y = points[0].y;
    }

    static constexpr uint8_t attack = 1;
    static constexpr uint8_t decay = 2;
    static constexpr uint8_t release = 3;

private:
};
