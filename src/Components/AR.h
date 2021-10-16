#pragma once

#include "Envelope.h"

class AR : public Envelope
{
public:
    AR()
    {
        points.push_back(Point());
        points.push_back(Point());
        points.push_back(Point());
		
        values.push_back(Value());
        values.push_back(Value());
    }

    virtual ~AR() = default;

    void computePoints(void)
    {
        float segmentWidth = getWidth() / 2;
        int16_t maxY = getHeight() - 1;

        // Starting point
        points[0].x = 0;
        points[0].y = maxY;

        // Attack
        points[1].x = segmentWidth * values[attack].value;
        points[1].y = 0;

        // Release
        points[2].x = points[1].x + segmentWidth * values[release].value;
        points[2].y = maxY;

        // Set the baseline
        baselineY = points[0].y;
    }

    static constexpr uint8_t attack = 0;
    static constexpr uint8_t release = 1;

private:
};
