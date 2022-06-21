#include "List.h"

List::List(const ListData *newListData) : AssignableList(newListData), index(0)
{
}

void List::setColour(uint32_t newColour)
{
    colour = newColour;
    repaint();
}

void List::setIndex(uint16_t newIndex)
{
    index = constrain(newIndex, 0, getList()->getMaxIndex());
    repaint();
}

void List::onTouchMove(const TouchEvent &touchEvent)
{
}

void List::onTouchDown(const TouchEvent &touchEvent)
{
}

void List::onTouchUp(const TouchEvent &touchEvent)
{
}

void List::onPotChange(const PotEvent &potEvent)
{
    setIndex(std::max(0, index + potEvent.getAcceleratedChange()));
}

void List::onPotTouchDown(const PotEvent &potEvent)
{
}

void List::onPotTouchUp(const PotEvent &potEvent)
{
}

void List::paint(Graphics &g)
{
    g.fillAll(LookAndFeel::backgroundColour);
    LookAndFeel::paintList(g, getBounds(), colour, getList(), index);
}

void List::resized(void)
{
    repaint();
}
