#include "SliderHorizontal.h"

SliderHorizontal::SliderHorizontal()
{
}

void SliderHorizontal::onTouchMove(const TouchEvent &touchEvent)
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

void SliderHorizontal::onTouchDown(const TouchEvent &touchEvent)
{
}

void SliderHorizontal::onTouchUp(const TouchEvent &touchEvent)
{
}

void SliderHorizontal::paint(Graphics &g)
{
    int16_t max = value.getMax();
    int16_t min = value.getMin();
    int16_t val = value.get();

    uint32_t colourTrack = Colours::darker(colour, 0.3f);

    uint16_t padding = (getHeight() - indicatorSize) / 2;
    uint16_t indicatorX = map(val, min, max, 0, getWidth() - indicatorSize);

    // Clear the component area
    g.fillAll(Colours::black);

    // Slider track
    g.setColour(colourTrack);
    g.fillRect(0, padding + indicatorRadius - 2, getWidth(), 4);

    // Indicator
    g.setColour(colour);
    g.fillElipse(
        indicatorX + indicatorRadius, padding + indicatorRadius, 11, 11);
}
