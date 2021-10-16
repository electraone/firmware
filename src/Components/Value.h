#pragma once

struct Value {
    Value() : value(0.0f), min(0.0f), max(1.0f)
    {
    }

    float value;
    float min;
    float max;
};
