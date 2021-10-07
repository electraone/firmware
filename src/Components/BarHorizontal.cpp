#include "BarHorizontal.h"

BarHorizontal::BarHorizontal()
{
}

void BarHorizontal::onTouchMove(const TouchEvent &touchEvent)
{
    float step = getWidth() / (float)(max - min);
    int16_t newValue =
        constrain(ceil(touchEvent.getX() / step + min), min, max);

    setValue(newValue);

    if (onValueChange) {
        onValueChange(value);
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
    uint16_t barHeight = getHeight() * 1.0f;
    uint16_t padding = (getHeight() - barHeight) / 2;

    uint16_t barX = map(0, min, max, 0, getWidth());
    uint16_t barWidth = map(value, min, max, 0, getWidth()) - barX;

    g.fillAll(Colours::black);

    // fader track background
    g.setColour(Colours::darkgrey);
    g.fillRect(0, padding, getWidth(), barHeight);

    // fader active bar
    g.setColour(Colours::orange);
    g.fillRect(barX, padding, barWidth, barHeight);
}
