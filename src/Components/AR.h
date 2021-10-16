#pragma once

#include "Envelope.h"

class AR : public Envelope
{
public:
    AR()
    {
        points.push_back(Point(0, 0));
        points.push_back(Point(0, 0));
        points.push_back(Point(0, 0));
        values.push_back(Value());
        values.push_back(Value());
    }

    virtual ~AR() = default;

    void computePoints(void)
    {
        float segmentWidth = getWidth() / 2;
        int16_t baseline = getHeight() - 1;

        // Starting point
        points[0].x = 0;
        points[0].y = baseline;

        // Attack
        points[1].x = segmentWidth * values[attack].value;
        points[1].y = 0;

        // Release
        points[2].x = points[1].x + segmentWidth * values[release].value;
        points[2].y = baseline;
    }

    static constexpr uint8_t attack = 0;
    static constexpr uint8_t release = 1;

private:
};
