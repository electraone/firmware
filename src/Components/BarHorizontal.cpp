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
    /*
    if (onValueChange) {
        onValueChange(value.get());
    }
    */
}

void BarHorizontal::onTouchDown(const TouchEvent &touchEvent)
{
}

void BarHorizontal::onTouchUp(const TouchEvent &touchEvent)
{
}

void BarHorizontal::paint(Graphics &g)
{
    g.fillAll(LookAndFeel::backgroundColour);
    LookAndFeel::paintBarHorizontal(
        g, getBounds(), colour, value.getMin(), value.getMax(), value.get());
}
