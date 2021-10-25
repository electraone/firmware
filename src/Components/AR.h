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
        points[1].x = segmentWidth * values[delay].getRelative();
        points[1].y = baselineY;

        // Attack
        points[2].x = points[1].x + segmentWidth * values[attack].getRelative();
        points[2].y = 0;

        // Release
        points[3].x =
            points[2].x + segmentWidth * values[release].getRelative();
        points[3].y = baselineY;
    }

    static constexpr uint8_t attack = 1;
    static constexpr uint8_t release = 2;

private:
};
