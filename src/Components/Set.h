#pragma once

#include "Component.h"
#include "LookAndFeel.h"

class Set : virtual public Component
{
public:
    Set() : colour(Colours565::white), label(""), isHighligted(false)
    {
    }

    Set(Rectangle newBounds,
        uint32_t newColour,
        const char *newLabel,
        bool newIsHighligted)
        : colour(newColour), label(newLabel), isHighligted(newIsHighligted)
    {
        setBounds(newBounds);
    }

    virtual ~Set() = default;

    void setColour(uint32_t newColour)
    {
        colour = newColour;
        repaint();
    }

    void setLabel(const char *newLabel)
    {
        label = newLabel;
        repaint();
    }

    void setHighlighted(bool shouldBeHighlighted)
    {
        isHighligted = shouldBeHighlighted;
        repaint();
    }

    void paint(Graphics &g)
    {
        LookAndFeel::paintSet(g, getBounds(), colour, label, isHighligted);
    }

protected:
    uint32_t colour;

private:
    static const uint8_t MaxNameLength = 40;
    const char *label;
    bool isHighligted;
};
