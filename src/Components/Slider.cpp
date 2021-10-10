#include "Slider.h"

Slider::Slider()
    : min(-64),
      max(63),
      value(min),
      colour(Colours::red),
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
    setValue(value);
}

int16_t Slider::getMinimum(void) const
{
    return (min);
}

void Slider::setMaximum(int16_t newMax)
{
    max = newMax;
    setValue(value);
}

int16_t Slider::getMaximum(void) const
{
    return (max);
}

void Slider::setRange(int16_t newMinimum, int16_t newMaximum)
{
    min = newMinimum;
    max = newMaximum;
    setValue(value);
}

void Slider::setValue(int16_t newValue)
{
    value = constrain(newValue, min, max);
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

void Slider::setColour(uint32_t newColour)
{
    colour = newColour;
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

void Slider::onPotChange(const PotEvent &potEvent)
{
    applyRelativeChange(potEvent.getRelativeChange());
}

void Slider::onPotTouchDown(const PotEvent &potEvent)
{
}

void Slider::onPotTouchUp(const PotEvent &potEvent)
{
}

void Slider::applyRelativeChange(int16_t relativeChange)
{
    value += relativeChange;
    setValue(value);
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
