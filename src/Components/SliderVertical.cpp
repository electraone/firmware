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
    LookAndFeel::paintSliderVertical(
        g, getBounds(), colour, value.getMin(), value.getMax(), value.get());
}
