#include "Envelope.h"

Envelope::Envelope() : activeSegment(1)
{
}

void Envelope::setColour(uint32_t newColour)
{
    colour = newColour;
    repaint();
}

void Envelope::setValue(uint8_t handle, float newValue)
{
    values[handle].value =
        constrain(newValue, values[handle].min, values[handle].max);
    repaint();
}

void Envelope::setMin(uint8_t handle, float newMin)
{
    values[handle].min = newMin;
    setValue(handle, values[handle].value);
}

void Envelope::setMax(uint8_t handle, float newMax)
{
    values[handle].max = newMax;
    setValue(handle, values[handle].value);
}

void Envelope::setActiveSegment(uint8_t newActiveSegment)
{
    activeSegment = newActiveSegment;
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
    if (potEvent.getRelativeChange() > 0) {
        setValue(activeSegment, values[activeSegment].value + 0.01f);
    } else {
        setValue(activeSegment, values[activeSegment].value - 0.01f);
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
    computePoints();
    paintContour(g);
}

void Envelope::paintContour(Graphics &g)
{
    uint32_t lighter = Colours::lighter(colour, 0.5f);
    uint32_t darker = Colours::darker(colour, 0.8f);

    // Paint the background
    g.fillAll(Colours::black);

    // Paint the base line
    g.setColour(Colours::darkgrey);
    g.drawLine(0, points[0].y, getWidth(), points[0].y);

    // Draw the envelope
    g.setColour(Colours::orange);

    for (uint8_t i = 0; i < std::max(0, (int)(points.size() - 1)); i += 1) {
        g.drawLine(points[i].x, points[i].y, points[i + 1].x, points[i + 1].y);
    }
}

void Envelope::resized(void)
{
    repaint();
}
