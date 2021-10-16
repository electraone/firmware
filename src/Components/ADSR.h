#pragma once

#include "Envelope.h"

class ADSR : public Envelope
{
public:
    ADSR()
    {
        points.push_back(Point(0, 0));
        points.push_back(Point(0, 0));
        points.push_back(Point(0, 0));
        points.push_back(Point(0, 0));
        points.push_back(Point(0, 0));
        values.push_back(Value());
        values.push_back(Value());
        values.push_back(Value());
        values.push_back(Value());
    }

    virtual ~ADSR() = default;

    void computePoints(void)
    {
        float segmentWidth = getWidth() / 4;
        int16_t baseline = 0;
        int16_t sustainLevel = 0;
        int16_t maxY = getHeight() - 1;

        baseline = map(0,
                       std::min((int)0, (int)(values[sustain].value * 256)),
                       values[sustain].max * 256,
                       maxY,
                       0);

        sustainLevel = map(values[sustain].value * 256,
                           std::min((int)0, (int)(values[sustain].value * 256)),
                           values[sustain].max * 256,
                           maxY,
                           0);

        // Starting point
        points[0].x = 0;
        points[0].y = baseline;

        // Attack
        points[1].x = segmentWidth * values[attack].value;
        points[1].y = 0;

        // Decay
        points[2].x = points[1].x + segmentWidth * values[decay].value;
        points[2].y = sustainLevel;

        // Sustain
        points[3].x = points[2].x + segmentWidth;
        points[3].y = points[2].y;

        // Release
        points[4].x = points[3].x + segmentWidth * values[release].value;
        points[4].y = points[0].y;
    }

    static constexpr uint8_t attack = 0;
    static constexpr uint8_t decay = 1;
    static constexpr uint8_t sustain = 2;
    static constexpr uint8_t release = 3;

private:
};
