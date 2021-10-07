#include "SliderVertical.h"

SliderVertical::SliderVertical()
{
}

void SliderVertical::onTouchMove(const TouchEvent &touchEvent)
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

void SliderVertical::onTouchDown(const TouchEvent &touchEvent)
{
}

void SliderVertical::onTouchUp(const TouchEvent &touchEvent)
{
}

void SliderVertical::paint(Graphics &g)
{
    uint16_t padding = (getWidth() - indicatorSize) / 2;
    uint16_t indicatorY = map(value, min, max, 0, getHeight() - indicatorSize);

    g.fillAll(Colours::black);

    // Slider track
    g.setColour(Colours::darkgrey);
    g.drawRect(padding + indicatorRadius - 1, 0, 2, getHeight());

    // Indicator
    g.setColour(Colours::white);
    g.fillElipse(padding + indicatorRadius,
                 getHeight() - indicatorY - indicatorRadius,
                 11,
                 11);
}
