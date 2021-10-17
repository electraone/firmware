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

    void computePoints(void)
    {
        uint16_t segmentWidth = getSegmentWidth(2);

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

        // Release
        points[3].x = points[2].x + segmentWidth * values[release].value;
        points[3].y = baselineY;
    }

    static constexpr uint8_t attack = 1;
    static constexpr uint8_t release = 2;

private:
};
