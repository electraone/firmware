#include "Envelope.h"
#include "LookAndFeel.h"

Envelope::Envelope()
    : activeSegment(1),
      boundaryMin(0.0f),
      boundaryMax(1.0f),
      baselineY(0),
      delayEnabled(false)
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

void Envelope::setMin(uint8_t handle, float newMin)
{
    values[handle].setMin(newMin);
    boundaryMin = (newMin < boundaryMin) ? newMin : boundaryMin;
}

void Envelope::setMax(uint8_t handle, float newMax)
{
    values[handle].setMax(newMax);
    boundaryMax = (newMax > boundaryMax) ? newMax : boundaryMax;
}

void Envelope::setActiveSegment(uint8_t newActiveSegment)
{
    activeSegment = newActiveSegment;
}

void Envelope::useDelay(bool shouldBeUsed)
{
    delayEnabled = shouldBeUsed;
}

void Envelope::onTouchMove(const TouchEvent &touchEvent)
{
}

void Envelope::onTouchDown(const TouchEvent &touchEvent)
{
}

void Envelope::onTouchUp(const TouchEvent &touchEvent)
{
}

void Envelope::onPotChange(const PotEvent &potEvent)
{
    if (potEvent.getRelativeChange()) {
        applyValue(activeSegment, potEvent.getRelativeChange());
    }
}

void Envelope::onPotTouchDown(const PotEvent &potEvent)
{
}

void Envelope::onPotTouchUp(const PotEvent &potEvent)
{
}

void Envelope::paint(Graphics &g)
{
    // If delay is disabled, set delay value to 0
    if (!delayEnabled) {
        values[delay].set(0.0f);
    }

    // Compute points on the envelope contour
    computePoints();

    LookAndFeel::paintEnvelope(g, getBounds(), colour, baselineY, points);
}

void Envelope::resized(void)
{
    repaint();
}

uint16_t Envelope::getSegmentWidth(uint8_t numSegments)
{
    return (getWidth() / (numSegments + delayEnabled));
}
