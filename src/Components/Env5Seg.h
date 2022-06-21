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

        values.push_back(Value()); // Level 1
        values.push_back(Value()); // Rate 1
        values.push_back(Value()); // Level 2
        values.push_back(Value()); // Rate 2
        values.push_back(Value()); // Level 3
        values.push_back(Value()); // Rate 3
        values.push_back(Value()); // Level 4
        values.push_back(Value()); // Rate 4
    }

    virtual ~Env5Seg() = default;

    float getMinimumLevel(void)
    {
        float minLevel =
            std::min(std::min(values[level1].get(), values[level2].get()),
                     std::min(values[level3].get(), values[level4].get()));
        if (minLevel > 0) {
            minLevel = 0;
        }

        return (minLevel);
    }

    float getMaximumLevel(void)
    {
        float maxLevel =
            std::max(std::max(values[level1].get(), values[level2].get()),
                     std::max(values[level3].get(), values[level4].get()));

        if (maxLevel < 0) {
            maxLevel = 0;
        }

        return (maxLevel);
    }

    void computePoints(const Rectangle &bounds)
    {
        uint16_t segmentWidth = getSegmentWidth(bounds.getWidth(), 5);
        float maxY = bounds.getHeight() - 1;

        float boundaryMin = getMinimumLevel();
        float boundaryMax = getMaximumLevel();

        int16_t baseline = map(0.0f, boundaryMin, boundaryMax, maxY, 0.0f);

        int16_t level1Y =
            map(values[level1].get(), boundaryMin, boundaryMax, maxY, 0.0f);
        int16_t level2Y =
            map(values[level2].get(), boundaryMin, boundaryMax, maxY, 0.0f);
        int16_t level3Y =
            map(values[level3].get(), boundaryMin, boundaryMax, maxY, 0.0f);
        int16_t level4Y =
            map(values[level4].get(), boundaryMin, boundaryMax, maxY, 0.0f);

        // Starting point
        points[0].x = 0;
        points[0].y = level4Y;

        // Delay
        points[1].x = segmentWidth * 0; //values[delay].getRelative();
        points[1].y = level4Y;

        // Segment 1
        points[2].x = points[1].x + segmentWidth * values[rate1].getRelative();
        points[2].y = level1Y;

        // Segment 2
        points[3].x = points[2].x + segmentWidth * values[rate2].getRelative();
        points[3].y = level2Y;

        // Segment 3
        points[4].x = points[3].x + segmentWidth * values[rate3].getRelative();
        points[4].y = level3Y;

        // Segment 4
        points[5].x = points[4].x + segmentWidth;
        points[5].y = level3Y;

        // Segment 5
        points[6].x = points[5].x + segmentWidth * values[rate4].getRelative();
        points[6].y = level4Y;

        // Set the baseline
        baselineY = baseline;
    }

    static constexpr uint8_t level1 = 0;
    static constexpr uint8_t rate1 = 1;
    static constexpr uint8_t level2 = 2;
    static constexpr uint8_t rate2 = 3;
    static constexpr uint8_t level3 = 4;
    static constexpr uint8_t rate3 = 5;
    static constexpr uint8_t level4 = 6;
    static constexpr uint8_t rate4 = 7;

private:
};
