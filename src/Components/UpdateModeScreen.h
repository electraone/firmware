#pragma once

#include "Component.h"

class UpdateModeScreen : public Component
{
public:
    UpdateModeScreen()
    {
        setBounds(0, 20, 1024, 560);
        setVisible(true);
    }

    virtual ~UpdateModeScreen() = default;

    void paint(Graphics &g)
    {
        g.setBteChromaColor(0x000000);
        g.dim(0, 0, 1024, 600, Colours565::black);
        g.dim(398, 162, 228, 205, 0x1840);
        g.copyFromToolkit(822, 78, 466, 200, 92, 80);
        g.printText(398,
                    320,
                    "Firmware Update mode",
                    TextStyle::mediumTransparent,
                    228,
                    TextAlign::center);
    }

private:
};
