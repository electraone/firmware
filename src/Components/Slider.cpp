#include "Slider.h"

Slider::Slider()
    : colour(Colours565::red), formatString("%d"), valueBoxPosition(NoValueBox)
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
    value.setMin(newMin);
    if (getValue() < newMin) {
        setValue(newMin);
    }
    repaint();
}

int16_t Slider::getMinimum(void) const
{
    return (value.getMin());
}

void Slider::setMaximum(int16_t newMax)
{
    value.setMax(newMax);
    if (getValue() > newMax) {
        setValue(newMax);
    }
    repaint();
}

int16_t Slider::getMaximum(void) const
{
    return (value.getMax());
}

void Slider::setRange(int16_t newMin, int16_t newMax)
{
    value.setRange(newMin, newMax);
    repaint();
}

void Slider::setValue(int16_t newValue)
{
    value.set(newValue);
    repaint();
}

void Slider::applyValue(int16_t delta)
{
    value.apply(delta);
    repaint();
}

int16_t Slider::getValue(void) const
{
    return (value.get());
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
    if (potEvent.getRelativeChange()) {
        applyValue(potEvent.getRelativeChange());
    }
}

void Slider::onPotTouchDown(const PotEvent &potEvent)
{
}

void Slider::onPotTouchUp(const PotEvent &potEvent)
{
}

void Slider::paintValueBox(Graphics &g)
{
    char stringValue[10];
    snprintf(stringValue, sizeof(stringValue), formatString, value.get());
    g.printText(0,
                getHeight() - 13,
                stringValue,
                TextStyle::mediumTransparent,
                getWidth(),
                TextAlign::center);
}
