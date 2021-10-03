#pragma once

#include "Component.h"

class ButtonComponent : public Component
{
public:
    ButtonComponent() : dimmed(false), colour(ElectraColours::white)
    {
    }

    ~ButtonComponent()
    {
    }

    // Lambdas
    std::function<void(void)> onClick;

    void setLabel(const char *newLabel)
    {
        copyString(label, newLabel, maxLabelLength);
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

    void onTouchDown(const TouchEvent &touchEvent)
    {
        this->onClick();
    }

    void paint(Graphics &g)
    {
        g.fillAll(ElectraColours::rgb565NumericBlack);

        if (active) {
            g.copyFromToolkit(924, 555, 0, 0, 98, 44);
            g.printText(0,
                        getHeight() / 2 - 7,
                        label,
                        TextStyle::mediumBlackOnWhite,
                        getWidth(),
                        TextAlign::center);
        } else {
            g.copyFromToolkit(924, 509, 0, 0, 98, 44);
            g.printText(0,
                        getHeight() / 2 - 7,
                        label,
                        TextStyle::mediumWhiteOnBlack,
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
        Colour colour;
        bool dimmed : 1;
    };
};
