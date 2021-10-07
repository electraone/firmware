#include "SliderHorizontal.h"

SliderHorizontal::SliderHorizontal()
{
}

void SliderHorizontal::onTouchMove(const TouchEvent &touchEvent)
{
    float step = getWidth() / (float)(max - min);
    int16_t newValue =
        constrain(ceil(touchEvent.getX() / step + min), min, max);

    setValue(newValue);

    if (onValueChange) {
        onValueChange(value);
    }
}

void SliderHorizontal::onTouchDown(const TouchEvent &touchEvent)
{
}

void SliderHorizontal::onTouchUp(const TouchEvent &touchEvent)
{
}

void SliderHorizontal::paint(Graphics &g)
{
    uint16_t padding = (getHeight() - indicatorSize) / 2;
    uint16_t indicatorX = map(value, min, max, 0, getWidth() - indicatorSize);

    g.fillAll(Colours::black);

    // Slider track
    g.setColour(Colours::darkgrey);
    g.drawRect(0, padding + indicatorRadius - 1, getWidth(), 2);

    // Indicator
    g.setColour(Colours::white);
    g.fillElipse(
        indicatorX + indicatorRadius, padding + indicatorRadius, 11, 11);
}
