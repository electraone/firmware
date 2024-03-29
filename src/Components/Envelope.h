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
 * @file Envelope.h
 *
 * @brief A base class for envelope components.
 */

#pragma once

#include "Component.h"

class Envelope : virtual public Component
{
public:
    Envelope();
    virtual ~Envelope() = default;

    void setColour(uint32_t newColour);

    virtual void setValue(uint8_t handle, float newValue);
    virtual void applyValue(uint8_t handle, float delta);
    virtual float getValue(uint8_t handle);
    virtual void setMin(uint8_t handle, float newMin);
    virtual void setMax(uint8_t handle, float newMax);

    void setActiveSegment(uint8_t newActiveSegment);
    void switchToNextActiveHandle(void);
    void switchToPreviousActiveHandle(void);
    uint8_t getActiveSegment(void) const;
    void showActiveSegment(bool shouldBeShown);
    void useDelay(bool shouldBeUsed);

    virtual void onTouchMove(const TouchEvent &touchEvent) override;
    virtual void onTouchDown(const TouchEvent &touchEvent) override;
    virtual void onTouchUp(const TouchEvent &touchEvent) override;

    virtual void onPotChange(const PotEvent &potEvent) override;
    virtual void onPotTouchDown(const PotEvent &potEvent) override;
    virtual void onPotTouchUp(const PotEvent &potEvent) override;

    virtual void computePoints(const Rectangle &bounds) = 0;
    virtual void resized(void) override;
    virtual void paint(Graphics &g) override;

    static constexpr uint8_t delay = 0;

protected:
    uint16_t getSegmentWidth(uint16_t width, uint8_t numSegments);

    struct {
        bool delayEnabled : 1;
        uint8_t activeSegment : 6;
        bool activeSegmentIsShown : 1;
    };

    std::vector<Point> points;
    std::vector<Value> values;
    int16_t baselineY;
    uint32_t colour;

private:
    void paintContour(Graphics &g);
    void paintBaseline(Graphics &g);
    void paintMarkers(Graphics &g);
    void paintFills(Graphics &g);

    static bool findIntersection(uint16_t lineY,
                                 Point &C,
                                 Point &D,
                                 Point &intersection);
};
