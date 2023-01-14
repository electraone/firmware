#pragma once

#include "Component.h"

class Circle final : public Component
{
public:
    explicit Circle(bool shouldBeFilled = false) : filled(shouldBeFilled)
    {
        setName("circle");
    }

    void paint(Graphics &g) override
    {
        uint16_t centreX = getWidth() / 2;
        uint16_t centreY = getHeight() / 2;
        uint16_t radius = getWidth() / 2 - 1;

        g.setColour(Colours565::white);

        if (filled) {
            g.fillElipse(centreX, centreY, radius, radius);
        } else {
            g.drawElipse(centreX, centreY, radius, radius);
        }
    }

    void onTouchDown(const TouchEvent &touchEvent) override
    {
        System::logger.write(ERROR, "callback: circle");
    }

private:
    bool filled;
};
