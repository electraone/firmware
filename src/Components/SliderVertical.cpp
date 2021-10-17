#include "SliderVertical.h"

SliderVertical::SliderVertical()
{
}

void SliderVertical::onTouchMove(const TouchEvent &touchEvent)
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

void SliderVertical::onTouchDown(const TouchEvent &touchEvent)
{
}

void SliderVertical::onTouchUp(const TouchEvent &touchEvent)
{
}

void SliderVertical::paint(Graphics &g)
{
    int16_t max = value.getMax();
    int16_t min = value.getMin();
    int16_t val = value.get();

    uint32_t colourTrack = Colours::darker(colour, 0.3f);

    uint16_t padding = (getWidth() - indicatorSize) / 2;
    uint16_t indicatorY = map(val, min, max, 0, getHeight() - indicatorSize);

    // Clear the component area
    g.fillAll(Colours::black);

    // Slider track
    g.setColour(colourTrack);
    g.fillRect(padding + indicatorRadius - 2, 0, 4, getHeight());

    // Indicator
    g.setColour(colour);
    g.fillElipse(padding + indicatorRadius,
                 getHeight() - indicatorY - indicatorRadius,
                 11,
                 11);
}
