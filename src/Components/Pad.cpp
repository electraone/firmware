#include "Pad.h"

Pad::Pad() : state (false)
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
    uint32_t colourOff = Colours::darker(colour, 0.1f);
	uint32_t colourOn = Colours::darker(colour, 0.8f);

    // Paint the background
    g.setColour(state == true ? colourOn : colourOff);
    g.fillRoundRect(0, 0, getWidth(), getHeight(), 8);

    // Paint the outline
	g.setColour(colour);
    g.drawRoundRect(0, 0, getWidth(), getHeight(), 8);
}

void Pad::resized(void)
{
    repaint();
}
