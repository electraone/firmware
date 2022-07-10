#include "MeterHorizontal.h"

MeterHorizontal::MeterHorizontal()
    : colourInactive(Colours::grey),
      colourBackground(LookAndFeel::backgroundColour)
{
}

void MeterHorizontal::setColourInactive(uint32_t newColour)
{
    colourInactive = newColour;
    repaint();
}

void MeterHorizontal::setColourBackground(uint32_t newColour)
{
    colourBackground = newColour;
    repaint();
}

void MeterHorizontal::onTouchMove(const TouchEvent &touchEvent)
{
}

void MeterHorizontal::onTouchDown(const TouchEvent &touchEvent)
{
}

void MeterHorizontal::onTouchUp(const TouchEvent &touchEvent)
{
}

void MeterHorizontal::paint(Graphics &g)
{
    g.fillAll(LookAndFeel::backgroundColour);
    LookAndFeel::paintMeterHorizontal(g,
                                      getBounds(),
                                      colour,
                                      colourInactive,
                                      colourBackground,
                                      value.getMin(),
                                      value.getMax(),
                                      value.get());
}
