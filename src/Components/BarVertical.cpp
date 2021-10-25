#include "BarVertical.h"

BarVertical::BarVertical()
{
}

void BarVertical::onTouchMove(const TouchEvent &touchEvent)
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

void BarVertical::onTouchDown(const TouchEvent &touchEvent)
{
}

void BarVertical::onTouchUp(const TouchEvent &touchEvent)
{
}

void BarVertical::paint(Graphics &g)
{
    LookAndFeel::paintBarVertical(
        g, getBounds(), colour, value.getMin(), value.getMax(), value.get());
}
