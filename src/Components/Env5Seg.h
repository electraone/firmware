#pragma once

#include "Envelope.h"

class Env5Seg : public Envelope
{
public:
    Env5Seg()
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
        values.push_back(Value());
        values.push_back(Value());
    }

    virtual ~Env5Seg() = default;

    void computePoints(void)
    {
        float segmentWidth = getWidth() / 5;
        float maxY = getHeight() - 1;

        int16_t baseline = map(0.0f, boundaryMin, boundaryMax, maxY, 0.0f);

        int16_t level1Level =
            map(values[level1].value, boundaryMin, boundaryMax, maxY, 0.0f);
        int16_t level2Level =
            map(values[level2].value, boundaryMin, boundaryMax, maxY, 0.0f);
        int16_t level3Level =
            map(values[level3].value, boundaryMin, boundaryMax, maxY, 0.0f);
        int16_t level4Level =
            map(values[level4].value, boundaryMin, boundaryMax, maxY, 0.0f);

        // Starting point
        points[0].x = 0;
        points[0].y = level4Level;

        // Segment 1
        points[1].x = segmentWidth * values[rate1].value;
        points[1].y = level1Level;

        // Segment 2
        points[2].x = points[1].x + segmentWidth * values[rate2].value;
        points[2].y = level2Level;

        // Segment 3
        points[3].x = points[2].x + segmentWidth * values[rate3].value;
        points[3].y = level3Level;

        // Segment 4
        points[4].x = points[3].x + segmentWidth;
        points[4].y = level3Level;

        // Segment 5
        points[5].x = points[4].x + segmentWidth * values[rate4].value;
        points[5].y = level4Level;

        // Set the baseline
        baselineY = baseline;
    }

    static constexpr uint8_t rate1 = 0;
    static constexpr uint8_t level1 = 1;
    static constexpr uint8_t rate2 = 2;
    static constexpr uint8_t level2 = 3;
    static constexpr uint8_t rate3 = 4;
    static constexpr uint8_t level3 = 5;
    static constexpr uint8_t rate4 = 6;
    static constexpr uint8_t level4 = 7;

private:
};
