#pragma once

#include "Component.h"

class Spinner : public Component
{
public:
    Spinner() : activeDot(0), dimmedApplied(false)
    {
        setBounds(0, 20, 1024, 560);
        setVisible(true);
    }

    virtual ~Spinner() = default;

    void reset(void)
    {
        dimmedApplied = false;
        activeDot = 0;
    }

    void paint(Graphics &g)
    {
        if (dimmedApplied == false) {
            g.dimAll();
            g.copyFromToolkit(814, 170, 448, 198, 128, 85);
            dimmedApplied = true;
        }

        g.setColour(Colours::black);
        g.fillRect(485, 298, 54, 6);

        for (uint8_t i = 0; i < 5; i++) {
            if (activeDot == i) {
                g.setColour(Colours::white);
            } else if ((activeDot == i - 1) || (activeDot == i + 1)) {
                g.setColour(0x630C);
            } else {
                g.setColour(0x2104);
            }
            g.fillCircle(488 + (i * 12), 320, 3);
        }
        activeDot++;

        if (activeDot > 4) {
            activeDot = 0;
        }
    }

private:
    uint8_t activeDot;
    bool dimmedApplied;
};
