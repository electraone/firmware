/*
* Electra One MIDI Controller Firmware
* See COPYRIGHT file at the top of the source tree.
*
* This product includes software developed by the
* Electra One Project (http://electra.one/).
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.
*/

/**
 * @file Spinner.h
 *
 * @brief A simple animated Spinner component.
 */

#pragma once

#include "Component.h"

class Spinner : public Component
{
public:
    Spinner() : activeDot(8), dimmedApplied(false)
    {
        setBounds(0, 20, 1024, 560);
        setVisible(true);
    }

    virtual ~Spinner() = default;

    void reset(void)
    {
        dimmedApplied = false;
        activeDot = 8;
    }

    void paint(Graphics &g)
    {
        g.setBteChromaColor(0x000000);

        if (dimmedApplied == false) {
            g.dimAll();
            g.copyFromToolkit(814, 170, 448, 198, 128, 85);
            dimmedApplied = true;
        }

        for (uint8_t i = 0; i < 9; i++) {
            uint8_t x = i + activeDot;
            if (x > 7) {
                x = x - 8;
            }
            if (1 < i && i < 7) {
                g.setColour(dotColours[x]);
                g.fillCircle(463 + (i * 12), 320, 3);
            }
        }
        activeDot--;

        if (activeDot < 0) {
            activeDot = 8;
        }
    }

private:
    int8_t activeDot;
    bool dimmedApplied;

    static constexpr uint32_t dotColours[9] = { 0x0000, 0x0861, 0x2104,
                                                0x630C, 0xFFFF, 0x630C,
                                                0x2104, 0x0861, 0x0000 };
};
