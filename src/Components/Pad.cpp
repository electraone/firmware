#include "Pad.h"

Pad::Pad() : state(false)
{
}

void Pad::setColour(uint32_t newColour)
{
    colour = newColour;
    repaint();
}

void Pad::setState(bool newState)
{
    state = newState;
    repaint();
}

bool Pad::getState(void) const
{
    return (state);
}

bool Pad::isDown(void) const
{
    return (state == true);
}

void Pad::onTouchMove(const TouchEvent &touchEvent)
{
}

void Pad::onTouchDown(const TouchEvent &touchEvent)
{
    state = !state;
    repaint();
}

void Pad::onTouchUp(const TouchEvent &touchEvent)
{
}

void Pad::onPotChange(const PotEvent &potEvent)
{
}

void Pad::onPotTouchDown(const PotEvent &potEvent)
{
}

void Pad::onPotTouchUp(const PotEvent &potEvent)
{
}

void Pad::paint(Graphics &g)
{
    LookAndFeel::paintPad(g, getBounds(), colour, false, state);
}

void Pad::resized(void)
{
    repaint();
}
