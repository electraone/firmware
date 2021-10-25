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
    LookAndFeel::paintSliderHorizontal(
        g, getBounds(), colour, value.getMin(), value.getMax(), value.get());
}
