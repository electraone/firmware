#include "BarVertical.h"

BarVertical::BarVertical()
{
}

void BarVertical::onTouchMove(const TouchEvent &touchEvent)
{
    float step = getHeight() / (float)(max - min);
    int16_t newValue =
        constrain((int16_t)ceil((getHeight() - touchEvent.getY()) / step + min),
                  min,
                  max);

    setValue(newValue);

    if (onValueChange) {
        onValueChange(value);
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
    uint16_t barWidth = getWidth() * 1.0f;
    uint16_t padding = (getWidth() - barWidth) / 2;

    uint16_t barY = map(0, min, max, 0, getHeight());
    uint16_t barHeight = map(value, min, max, 0, getHeight()) - barY;

    g.fillAll(Colours::black);

    // fader track background
    g.setColour(Colours::darkgrey);
    g.fillRect(padding, 0, barWidth, getHeight());

    // fader active bar
    g.setColour(Colours::orange);
    g.fillRect(padding, getHeight() - barY, barWidth, -barHeight);
}
