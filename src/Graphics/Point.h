#pragma once

#include <cstdint>

struct Point {
    Point() : x(0), y(0)
    {
    }

    Point(int16_t newX, int16_t newY) : x(newX), y(newY)
    {
    }

    int16_t x;
    int16_t y;
};
