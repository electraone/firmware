#pragma once

#include "Component.h"
#include <vector>

struct ListItem
{
	ListItem(uint16_t newValue, const char *newLabel) :
        value (newValue), label(newLabel) {}

	uint16_t value;
	String label;
};

class List : public Component
{
public:
	List();
	virtual ~List() = default;
	void setColour(uint32_t newColour);
	void setIndex(uint16_t newIndex);
	void assignListItems(const std::vector<ListItem> &newItems);

	virtual void onTouchMove(const TouchEvent &touchEvent) override;
	virtual void onTouchDown(const TouchEvent &touchEvent) override;
	virtual void onTouchUp(const TouchEvent &touchEvent) override;

	virtual void onPotChange(const PotEvent &potEvent) override;
	virtual void onPotTouchDown(const PotEvent &potEvent) override;
	virtual void onPotTouchUp(const PotEvent &potEvent) override;

	virtual void resized(void) override;
    virtual void paint(Graphics &g) override;

private:
	void paintDots(Graphics &g);
	void paintBar(Graphics &g);

    uint32_t colour;
	std::vector<ListItem> &items;
	uint16_t index;

	static std::vector<ListItem> empty;
};
