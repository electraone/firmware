#pragma once

#include "Component.h"

class SelectorComponent : public Component
{
public:
    SelectorComponent() : dimmed(false), colour(ElectraColours::white)
    {
    }

    ~SelectorComponent()
    {
    }

    // Lambdas
    std::function<void(void)> onClick;

    void setLabel(const char *newLabel)
    {
        copyString(label, newLabel, maxLabelLength);
    }

    const char *getLabel(void)
    {
        return (label);
    }

    void setDimmed(bool shouldBeDimmed)
    {
        dimmed = shouldBeDimmed;
    }

    bool isDimmed(void) const
    {
        return (dimmed);
    }

    void setColour(Colour newColour)
    {
        colour = newColour;
    }

    Colour getColour(void)
    {
        return (colour);
    }

    void onTouchDown(const TouchEvent &touchEvent)
    {
        this->onClick();
    }

    void onTouchUp(const TouchEvent &touchEvent)
    {
    }

    void paint(Graphics &g)
    {
        uint16_t textOffsetY = (getHeight() / 2) - 6;
        uint16_t rgb565NumericDarkerColour =
            ElectraColours::getNumericRgb565Darker(colour);

        g.fillAll(ElectraColours::rgb565NumericBlack);
        g.setColour(rgb565NumericDarkerColour);

        if (active == true) {
            g.fillRect(0, 0, getWidth(), getHeight());
            g.printText(0,
                        textOffsetY,
                        label,
                        TextStyle::mediumTransparent,
                        getWidth(),
                        TextAlign::center);
        } else {
            g.drawRect(0, 0, getWidth(), getHeight());
            g.drawRect(1, 1, getWidth() - 2, getHeight() - 2);
            g.printText(0,
                        textOffsetY,
                        label,
                        TextStyle::mediumTransparent,
                        getWidth(),
                        TextAlign::center);
        }

        if (dimmed == true) {
            g.dimAll();
        }
    }

private:
    static const uint8_t maxLabelLength = 20;
    char label[maxLabelLength + 1];

    struct {
        Colour colour : 3;
        bool dimmed : 1;
    };
};
