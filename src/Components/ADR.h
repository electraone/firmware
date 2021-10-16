#pragma once

#include "Envelope.h"

class ADR : public Envelope
{
public:
    ADR()
    {
        points.push_back(Point());
        points.push_back(Point());
        points.push_back(Point());
        points.push_back(Point());
		
        values.push_back(Value());
        values.push_back(Value());
        values.push_back(Value());
    }

    virtual ~ADR() = default;

    void computePoints(void)
    {
        float segmentWidth = getWidth() / 3;
        int16_t maxY = getHeight() - 1;
        int16_t decayLength = segmentWidth * values[decay].value;
        float factor = (float)decayLength / (float)segmentWidth;

        // Starting point
        points[0].x = 0;
        points[0].y = maxY;

        // Attack
        points[1].x = segmentWidth * values[attack].value;
        points[1].y = 0;

        // Decay
        points[2].x = points[1].x + decayLength;
        points[2].y = maxY - (maxY * factor);

        // Release
        points[3].x = points[2].x + segmentWidth * values[release].value;
        points[3].y = points[0].y;

        // Set the baseline
        baselineY = points[0].y;
    }

    static constexpr uint8_t attack = 0;
    static constexpr uint8_t decay = 1;
    static constexpr uint8_t release = 2;

private:
};
