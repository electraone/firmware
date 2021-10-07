#include "Slider.h"

Slider::Slider()
    : min(0),
      max(127),
      value(min),
      formatString("%d"),
      valueBoxPosition(NoValueBox)
{
}

void Slider::setValueBoxPosition(Slider::ValueBoxPosition newValueBoxPosition)
{
    valueBoxPosition = newValueBoxPosition;
    repaint();
}

Slider::ValueBoxPosition Slider::getValueBoxPosition(void) const
{
    return (valueBoxPosition);
}

void Slider::setMinimum(int16_t newMin)
{
    min = newMin;
    repaint();
}

int16_t Slider::getMinimum(void) const
{
    return (min);
}

void Slider::setMaximum(int16_t newMax)
{
    min = newMax;
    repaint();
}

int16_t Slider::getMaximum(void) const
{
    return (max);
}

void Slider::setRange(int16_t newMinimum, int16_t newMaximum)
{
    min = newMinimum;
    max = newMaximum;
    repaint();
}

void Slider::setValue(int16_t newValue)
{
    value = newValue;
    repaint();
}

int16_t Slider::getValue(int16_t value) const
{
    return (value);
}

void Slider::setValueFormat(const char *newValueFormat)
{
    formatString = newValueFormat;
    repaint();
}

void Slider::resized(void)
{
    repaint();
}

void Slider::onTouchMove(const TouchEvent &touchEvent)
{
}

void Slider::onTouchDown(const TouchEvent &touchEvent)
{
}

void Slider::onTouchUp(const TouchEvent &touchEvent)
{
}

void Slider::paintValueBox(Graphics &g)
{
    char stringValue[10];
    snprintf(stringValue, sizeof(stringValue), formatString, value);
    g.printText(0,
                getHeight() - 13,
                stringValue,
                TextStyle::mediumTransparent,
                getWidth(),
                TextAlign::center);
}
