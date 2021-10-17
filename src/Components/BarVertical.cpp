#include "BarVertical.h"

BarVertical::BarVertical()
{
}

void BarVertical::onTouchMove(const TouchEvent &touchEvent)
{
    int16_t max = value.getMax();
    int16_t min = value.getMin();

    float step = getHeight() / (float)(max - min);
    int16_t newValue =
        constrain((int16_t)ceil((getHeight() - touchEvent.getY()) / step + min),
                  min,
                  max);

    setValue(newValue);

    if (onValueChange) {
        onValueChange(value.get());
    }
}

void BarVertical::onTouchDown(const TouchEvent &touchEvent)
{
}

void BarVertical::onTouchUp(const TouchEvent &touchEvent)
{
}

void BarVertical::paint(Graphics &g)
{
    int16_t max = value.getMax();
    int16_t min = value.getMin();
    int16_t val = value.get();

    uint32_t colourTrack = Colours::darker(colour, 0.3f);

    uint16_t barWidth = getWidth() * 1.0f;
    uint16_t padding = (getWidth() - barWidth) / 2;

    uint16_t barY = map(0, min, max, 0, getHeight());
    uint16_t barHeight = map(val, min, max, 0, getHeight()) - barY;

    // Clear the component area
    g.fillAll(Colours::black);

    // Paint the track background
    g.setColour(colourTrack);
    g.fillRect(padding, 0, barWidth, getHeight());

    // Paint the active bar
    g.setColour(colour);
    g.fillRect(padding, getHeight() - barY, barWidth, -barHeight);

    // Paint the outline
    g.drawRect(padding, 0, barWidth, getHeight());
}
