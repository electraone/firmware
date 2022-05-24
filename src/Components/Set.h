#pragma once

#include "Component.h"
#include "LookAndFeel.h"

class Set : virtual public Component
{
public:
    Set() : colour(Colours::white), label("")
    {
    }

    Set(Rectangle newBounds, uint32_t newColour, const char *newLabel)
        : colour(newColour), label(newLabel)
    {
        setBounds(newBounds);
    }

    virtual ~Set() = default;

    void setColour(uint32_t newColour)
    {
        colour = newColour;
    }

    void setLabel(const char *newLabel)
    {
        label = newLabel;
    }

    void paint(Graphics &g)
    {
        LookAndFeel::paintSet(g, getBounds(), colour, label);
    }

private:
    static const uint8_t MaxNameLength = 40;
    uint32_t colour;
    const char *label;
};
