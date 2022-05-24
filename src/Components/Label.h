#pragma once

#include "Component.h"

class Label : virtual public Component
{
public:
    Label() : isDisabled(false), active(false), backgroundColour(0x0000)
    {
        *label = '\0';
    }

    ~Label()
    {
    }

    // Lambdas
    std::function<bool(void)> onClick;
    std::function<bool(void)> onRelease;

    void setLabel(const char *newLabel)
    {
        if (strlen(newLabel) == 0) {
            *label = '\0';
        } else {
            copyString(label, newLabel, maxLabelLength);
        }
    }

    void setBackgroundColour(uint16_t newBackgroundColour)
    {
        backgroundColour = newBackgroundColour;
    }

    void setDisabled(bool shouldBeDisabled)
    {
        isDisabled = shouldBeDisabled;
    }

    void setActive(bool shouldBeActive)
    {
        active = shouldBeActive;
    }

    void paint(Graphics &g)
    {
        uint16_t labelWidth =
            g.getTextWidth(label, TextStyle::smallWhiteOnBlack);

        g.printText(0,
                    0,
                    label,
                    TextStyle::smallTransparent,
                    getWidth(),
                    TextAlign::center);

        if (active == true) {
            g.setColour(Colours::white);
            g.drawLine((getWidth() / 2) - (labelWidth / 2) - 1,
                       15,
                       (getWidth() / 2) - (labelWidth / 2) - 1 + labelWidth,
                       15);
        }
    }

    void onPotTouchDown(const PotEvent &potEvent) override
    {
        if (onClick) {
            onClick();
        }
    }

    void onPotTouchUp(const PotEvent &potEvent) override
    {
        if (onRelease) {
            onRelease();
        }
    }

private:
    static const uint8_t maxLabelLength = 20;
    char label[maxLabelLength + 1];
    uint16_t backgroundColour;
    bool active;

    struct {
        bool isDisabled : 1;
    };
};
