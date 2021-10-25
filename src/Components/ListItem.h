#pragma once

#include <cstdint>
#include <string>

struct ListItem {
    ListItem(uint16_t newValue, const char *newLabel)
        : value(newValue), label(newLabel)
    {
    }

    uint16_t value;
    String label;
};
