#pragma once

#include "Component.h"

class VoidSpace : public Component
{
public:
    VoidSpace()
    {
    }

    virtual ~VoidSpace() = default;

    void onTouchDown(const TouchEvent &touchEvent) override
    {
        if (onClick) {
            onClick();
        }
    }

    void paint(Graphics &g) override
    {
    }

    std::function<void(void)> onClick;

private:
};
