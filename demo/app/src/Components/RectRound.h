#pragma once

#include "Component.h"

class RectRound final : public Component
{
public:
    explicit RectRound(bool shouldBeFilled = false) : filled(shouldBeFilled)
    {
        setName("rectRound");
    }

    void paint(Graphics &g) override
    {
        g.setColour(Colours565::red);

        if (filled) {
            g.fillRoundRect(0, 0, getWidth(), getHeight(), 20);
        } else {
            g.drawRoundRect(0, 0, getWidth(), getHeight(), 20);
        }
    }

    void onTouchDown(const TouchEvent &touchEvent) override
    {
        System::logger.write(LOG_ERROR, "callback: rectRound");
    }

private:
    bool filled;
};
