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
    bool longHold;

    bool isInCloseRange(void)
    {
      return ((abs(yStart - y) < CLICK_DISTANCE) && (abs(xStart - x) < CLICK_DISTANCE));
    }
};
