#pragma once

#include "Component.h"

class Rect final : public Component
{
public:
    explicit Rect(bool shouldBeFilled = false) : filled(shouldBeFilled)
    {
        setName("rect");
    }

    void paint(Graphics &g) override
    {
        g.setColour(Colours565::orange);

        if (filled) {
            g.fillRect(0, 0, getWidth(), getHeight());
        } else {
            g.drawRect(0, 0, getWidth(), getHeight());
        }
    }

    void onTouchDown(const TouchEvent &touchEvent) override
    {
        System::logger.write(LOG_ERROR, "callback: rect");
    }

private:
    bool filled;
};
