#pragma once

#include "Component.h"

class Indicator final : public Component
{
public:
    Indicator()
    {
        setName("indicator");
    }

    void paint(Graphics &g) override
    {
        g.dim(0, 0, getWidth(), 100, 0x0007);
        g.setColour(ElectraColours::rgb565NumericWhite);
        g.fillTriangle(getWidth() / 2,
                       getHeight() - 55,
                       getWidth() / 2 - 10,
                       getHeight() - 45,
                       getWidth() / 2 + 10,
                       getHeight() - 45);
        g.printText(0,
                    getHeight() - 38,
                    "Touch Me",
                    TextStyle::smallTransparent,
                    getWidth(),
                    TextAlign::center);
    }

    void onTouchDown(const TouchEvent &touchEvent) override
    {
        logMessage("callback: indicator");
    }
};
