#pragma once

#include "Component.h"

class LabelComponent : public Component
{
public:
    LabelComponent()
        : isDisabled(false), active(false), backgroundColour(0x0000)
    {
        *label = '\0';
    }

    ~LabelComponent()
    {
    }

    // Lambdas
    std::function<bool(void)> onClick;

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

    void paint(Graphics &g)
    {
        uint16_t labelWidth =
            g.getTextWidth(label, TextStyle::smallWhiteOnBlack);

        g.printText(0,
                    0,
                    label,
                    TextStyle::smallWhiteOnBlack,
                    getWidth(),
                    TextAlign::center);

        if (active == true) {
            g.drawLine((getWidth() / 2) - (labelWidth / 2) - 1,
                       15,
                       (getWidth() / 2) - (labelWidth / 2) - 1 + labelWidth,
                       15,
                       0xFFFF);
        }
    }

    void onPotTouchDown(handle_t handle) override
    {
        if (onClick) {
            onClick();
        }
    }

    void onPotTouchUp(handle_t handle) override
    {
    }

private:
    static const uint8_t maxLabelLength = 20;
    char label[maxLabelLength + 1];
    uint16_t backgroundColour;

    struct {
        bool isDisabled : 1;
    };
};
