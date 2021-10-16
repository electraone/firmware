#pragma once

#include "Envelope.h"

class ADSSR : public Envelope
{
public:
    ADSSR()
    {
        points.push_back(Point());
        points.push_back(Point());
        points.push_back(Point());
        points.push_back(Point());
        points.push_back(Point());
        points.push_back(Point());

        values.push_back(Value());
        values.push_back(Value());
        values.push_back(Value());
        values.push_back(Value());
        values.push_back(Value());
        values.push_back(Value());
    }

    virtual ~ADSSR() = default;

    void computePoints(void)
    {
        float segmentWidth = getWidth() / 5;
        float maxY = getHeight() - 1;

        // Find the lowest point of the envelope
        float lowestY =
            std::min(values[breakPoint].value, values[sustain].value);

        // Set the baseline
        baselineY =
            map(0.0f, std::min(0.0f, lowestY), values[sustain].max, maxY, 0.0f);

        int16_t sustainLevel = map(values[sustain].value,
                                   std::min(0.0f, values[sustain].value),
                                   values[sustain].max,
                                   maxY,
                                   0.0f);

        int16_t breakLevel = map(values[breakPoint].value,
                                 std::min(0.0f, values[breakPoint].value),
                                 values[breakPoint].max,
                                 maxY,
                                 0.0f);

        // Starting point
        points[0].x = 0;
        points[0].y = baselineY;

        // Attack
        points[1].x = segmentWidth * values[attack].value;
        points[1].y = 0;

        // Decay
        points[2].x = points[1].x + segmentWidth * values[decay].value;
        points[2].y = breakLevel;

        // Slope
        points[3].x = points[2].x + segmentWidth * values[slope].value;
        points[3].y = sustainLevel;

        // Sustain
        points[4].x = points[3].x + segmentWidth;
        points[4].y = points[3].y;

        // Release
        points[5].x = points[4].x + segmentWidth * values[release].value;
        points[5].y = points[0].y;
    }

    static constexpr uint8_t attack = 0;
    static constexpr uint8_t decay = 1;
    static constexpr uint8_t breakPoint = 2;
    static constexpr uint8_t slope = 3;
    static constexpr uint8_t sustain = 4;
    static constexpr uint8_t release = 5;

private:
};
