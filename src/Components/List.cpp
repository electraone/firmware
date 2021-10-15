#include "List.h"

std::vector<ListItem> List::empty;

List::List() : index (0), items (empty)
{
}

void List::setColour(uint32_t newColour)
{
    colour = newColour;
    repaint();
}

void List::setIndex(uint16_t newIndex)
{
	index = constrain (newIndex, 0, items.size() - 1);
	repaint();
}

void List::assignListItems(const std::vector<ListItem> &newItems)
{
	items = newItems;
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
	setIndex(std::max(0, index + potEvent.getRelativeChange()));
}

void List::onPotTouchDown(const PotEvent &potEvent)
{
}

void List::onPotTouchUp(const PotEvent &potEvent)
{
}

void List::paint(Graphics &g)
{
	// Paint the background
    g.fillAll(Colours::black);
	g.setColour(Colours::grey);
	g.drawRect(0, 0, getWidth(), getHeight());
	//g.drawLine(getWidth() / 2, 0, getWidth() / 2, getHeight());

    // Print the label
	g.printText(0,
				getHeight() / 2 - 22,
				items[index].label.c_str(),
				TextStyle::mediumTransparent,
				getWidth(),
				TextAlign::center);

    // Paint the graphics
    if (items.size() < 16) {
        paintDots(g);
    }
	else {
        paintBar(g);
	}
}

void List::resized(void)
{
    repaint();
}

void List::paintDots(Graphics &g)
{
	// Paint the dots
	uint8_t paddingDots = (getWidth() - (items.size() * 8)) / 2 + 4;

	// paint dots
	for (size_t i = 0; i < items.size(); i++)
	{
		g.setColour((i == index) ? colour : Colours::darker(colour, 0.5));
		g.fillCircle(paddingDots + i * 8, getHeight() / 2, 2);
	}
}

void List::paintBar(Graphics &g)
{
	uint16_t lastItem = items.size() - 1;
	uint32_t dark = Colours::darker(colour, 0.5);
	uint32_t light = Colours::lighter(colour, 0.5);

	uint16_t paddingFader = (getWidth() - 137) / 2;
	float step = 127.0f / (float) lastItem;
	float faderLength = std::max(1.0f, abs (step * (index)));

    // Paint the track
	g.setColour(dark);
	g.fillRect(paddingFader + 4, getHeight() / 2 - 2, 129, 5);

    // Paint the start point
	g.setColour((index == 0) ? light : dark);
	g.fillCircle(paddingFader + 2, getHeight() / 2, 2);

    // Paint the active point on the track
    if ((index != 0) && (index != lastItem)) {
		g.setColour((index == lastItem) ? light : dark);
		g.fillCircle(paddingFader + 134, getHeight() / 2, 2);
    }

    // Paint the end point
	g.setColour(light);
	g.fillRect(paddingFader + faderLength, getHeight() / 2 - 2, step, 5);
}
