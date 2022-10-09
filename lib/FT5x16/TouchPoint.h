#pragma once

#include <cstdint>
#include <cmath>
#include "setup.h"

struct TouchPoint {
    enum Event { None, Start, Move, End, LongHold, Click, DoubleClick };

    uint16_t x;
    uint16_t y;
    uint16_t xStart;
    uint16_t yStart;
    uint8_t id;
    Event event;
    uint32_t tsStart;
    uint32_t tsPreviousClick;
    uint16_t prevX;
    uint16_t prevY;
    bool longHold;
    bool moved;

    bool isInCloseRange(void)
    {
        return (pointsInCloseRange(xStart, yStart, x, y));
    }

    static bool
        pointsInCloseRange(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
    {
        return ((abs(x1 - x2) < CLICK_DISTANCE)
                && (abs(y1 - y2) < CLICK_DISTANCE));
    }
};
