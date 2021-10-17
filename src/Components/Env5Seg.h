#pragma once

#include "Envelope.h"

class Env5Seg : public Envelope
{
public:
    Env5Seg()
    {
        points.push_back(Point()); // Stage 1
        points.push_back(Point()); // Stage 2
        points.push_back(Point()); // Stage 3
        points.push_back(Point()); // Stage 4
        points.push_back(Point()); // Stage 5

        values.push_back(Value()); // Rate 1
        values.push_back(Value()); // Level 1
        values.push_back(Value()); // Rate 2
        values.push_back(Value()); // Level 2
        values.push_back(Value()); // Rate 3
        values.push_back(Value()); // Level 3
        values.push_back(Value()); // Rate 4
        values.push_back(Value()); // Level 4
    }

    virtual ~Env5Seg() = default;

    void computePoints(void)
    {
        uint16_t segmentWidth = getSegmentWidth(5);
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

        // Delay
        points[1].x = segmentWidth * values[delay].value;
        points[1].y = level4Level;

        // Segment 1
        points[2].x = points[1].x + segmentWidth * values[rate1].value;
        points[2].y = level1Level;

        // Segment 2
        points[3].x = points[2].x + segmentWidth * values[rate2].value;
        points[3].y = level2Level;

        // Segment 3
        points[4].x = points[3].x + segmentWidth * values[rate3].value;
        points[4].y = level3Level;

        // Segment 4
        points[5].x = points[4].x + segmentWidth;
        points[5].y = level3Level;

        // Segment 5
        points[6].x = points[5].x + segmentWidth * values[rate4].value;
        points[6].y = level4Level;

        // Set the baseline
        baselineY = baseline;
    }

    static constexpr uint8_t rate1 = 1;
    static constexpr uint8_t level1 = 2;
    static constexpr uint8_t rate2 = 3;
    static constexpr uint8_t level2 = 4;
    static constexpr uint8_t rate3 = 5;
    static constexpr uint8_t level3 = 6;
    static constexpr uint8_t rate4 = 7;
    static constexpr uint8_t level4 = 8;

private:
};
