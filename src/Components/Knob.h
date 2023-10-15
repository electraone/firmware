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
 * @file Knob.h
 *
 * @brief A round Dial-style Knob component.
 */

#pragma once

#include "Slider.h"

class Knob : public Slider
{
public:
    explicit Knob();
    virtual ~Knob() override = default;

    virtual void onTouchMove(const TouchEvent &touchEvent) override;
    virtual void onTouchDown(const TouchEvent &touchEvent) override;
    virtual void onTouchUp(const TouchEvent &touchEvent) override;

    void paint(Graphics &g) override;

private:
    void paintValue(Graphics &g,
                    uint16_t xCentre,
                    uint16_t yCentre,
                    uint16_t radius,
                    float angleZero,
                    float angle);

    void drawCurve(Graphics &g,
                   uint16_t xCentre,
                   uint16_t yCentre,
                   uint16_t radius,
                   float angleStart,
                   float angleEnd);

    void maskUnused(Graphics &g,
                    uint16_t xCentre,
                    uint16_t yCentre,
                    uint8_t quadrant,
                    uint8_t quadrantStart,
                    uint8_t quadrantEnd);

    static void maskCurveSeqment(Graphics &g,
                                 uint16_t x1,
                                 uint16_t y1,
                                 uint16_t x2,
                                 uint16_t y2,
                                 uint16_t x3,
                                 uint16_t y3,
                                 uint16_t x4,
                                 uint16_t y4);
    static uint8_t getQuadrant(float angle);

    static constexpr float angleTrackStart = 2.007f;
    static constexpr float angleTrackEnd = 7.418f;
};
