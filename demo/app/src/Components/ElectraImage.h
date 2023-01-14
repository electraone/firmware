#pragma once

#include "Component.h"

class ElectraImage final : public Component
{
public:
    ElectraImage()
    {
        setName("electraImage");
    }

    void paint(Graphics &g) override
    {
        g.copyFromToolkit(820, 78, 0, 0, 94, 80);
    }

    void onTouchDown(const TouchEvent &touchEvent) override
    {
        System::logger.write(ERROR, "callback: electraImage");
    }
};
