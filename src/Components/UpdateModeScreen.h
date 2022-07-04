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
        g.dimAll();
        g.backdrop(398, 162, 228, 205, 0x3840);
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
