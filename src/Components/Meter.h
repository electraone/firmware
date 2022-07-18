#pragma once

#include "Component.h"

class Meter : public Slider
{
public:
    Indicator() : value(0), max(100)
    {
    }

    virtual ~Indicator() = default;

    void setValue(float newValue)
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
