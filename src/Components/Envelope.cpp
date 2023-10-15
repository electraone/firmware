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
 * @file Envelope.cpp
 *
 * @brief A base class for envelope components.
 */

#include "Envelope.h"
#include "LookAndFeel.h"

Envelope::Envelope()
    : delayEnabled(false),
      activeSegment(1),
      activeSegmentIsShown(false),
      baselineY(0)
{
    points.push_back(Point()); // Starting point
    points.push_back(Point()); // Delay

    values.push_back(Value()); // Delay
}

void Envelope::setColour(uint32_t newColour)
{
    colour = newColour;
    repaint();
}

void Envelope::setValue(uint8_t handle, float newValue)
{
    values[handle].set(newValue);
    repaint();
}

void Envelope::applyValue(uint8_t handle, float delta)
{
    values[handle].apply(delta);
    repaint();
}

float Envelope::getValue(uint8_t handle)
{
    return (values[handle].get());
}

void Envelope::setMin(uint8_t handle, float newMin)
{
    if (values[handle].get() < newMin) {
        values[handle].set(newMin);
    }
    values[handle].setMin(newMin);
}

void Envelope::setMax(uint8_t handle, float newMax)
{
    if (values[handle].get() > newMax) {
        values[handle].set(newMax);
    }
    values[handle].setMax(newMax);
}

void Envelope::setActiveSegment(uint8_t newActiveSegment)
{
    activeSegment = newActiveSegment;
}

void Envelope::showActiveSegment(bool shouldBeShown)
{
    activeSegmentIsShown = shouldBeShown;
    repaint();
}

void Envelope::switchToNextActiveHandle(void)
{
    if (activeSegment
        < (points.size() - 3)) { // without start, delay and last point
        activeSegment++;
        setActiveSegment(activeSegment);
        repaint();
    }
}

void Envelope::switchToPreviousActiveHandle(void)
{
    if (activeSegment > 0) {
        activeSegment--;
        setActiveSegment(activeSegment);
        repaint();
    }
}

uint8_t Envelope::getActiveSegment(void) const
{
    return (activeSegment);
}

void Envelope::useDelay(bool shouldBeUsed)
{
    delayEnabled = shouldBeUsed;
}

void Envelope::onTouchMove([[maybe_unused]] const TouchEvent &touchEvent)
{
}

void Envelope::onTouchDown([[maybe_unused]] const TouchEvent &touchEvent)
{
}

void Envelope::onTouchUp([[maybe_unused]] const TouchEvent &touchEvent)
{
}

void Envelope::onPotChange(const PotEvent &potEvent)
{
    if (potEvent.getRelativeChange()) {
        applyValue(activeSegment, potEvent.getRelativeChange());
    }
}

void Envelope::onPotTouchDown([[maybe_unused]] const PotEvent &potEvent)
{
}

void Envelope::onPotTouchUp([[maybe_unused]] const PotEvent &potEvent)
{
}

void Envelope::paint(Graphics &g)
{
    // If delay is disabled, set delay value to 0
    if (!delayEnabled) {
        values[delay].set(0.0f);
    }

    // Compute points on the envelope contour
    computePoints(getLocalBounds());
    g.fillAll(LookAndFeel::backgroundColour);
    LookAndFeel::paintEnvelope(g,
                               getLocalBounds(),
                               colour,
                               baselineY,
                               points,
                               activeSegment,
                               activeSegmentIsShown);
}

void Envelope::resized(void)
{
    repaint();
}

uint16_t Envelope::getSegmentWidth(uint16_t width, uint8_t numSegments)
{
    return (width / (numSegments + delayEnabled));
}
