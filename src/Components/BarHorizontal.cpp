#include "BarHorizontal.h"

BarHorizontal::BarHorizontal()
{
}

void BarHorizontal::onTouchMove(const TouchEvent &touchEvent)
{
    int16_t max = value.getMax();
    int16_t min = value.getMin();

    float step = getWidth() / (float)(max - min);
    int16_t newValue =
        constrain(ceil(touchEvent.getX() / step + min), min, max);

    setValue(newValue);

    if (onValueChange) {
        onValueChange(value.get());
    }
}

void BarHorizontal::onTouchDown(const TouchEvent &touchEvent)
{
}

void BarHorizontal::onTouchUp(const TouchEvent &touchEvent)
{
}

void BarHorizontal::paint(Graphics &g)
{
    int16_t max = value.getMax();
    int16_t min = value.getMin();
    int16_t val = value.get();

    uint32_t colourTrack = Colours::darker(colour, 0.3f);

    uint16_t barHeight = getHeight() * 1.0f;
    uint16_t padding = (getHeight() - barHeight) / 2;

    uint16_t barX = map(0, min, max, 0, getWidth());
    uint16_t barWidth = map(val, min, max, 0, getWidth()) - barX;

    // Clear the component area
    g.fillAll(Colours::black);

    // Paint the track background
    g.setColour(colourTrack);
    g.fillRect(0, padding, getWidth(), barHeight);

    // Paint the active bar
    g.setColour(colour);
    g.fillRect(barX, padding, barWidth, barHeight);

    // Paint the outline
    g.drawRect(0, padding, getWidth(), barHeight);
}
