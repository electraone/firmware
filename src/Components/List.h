#pragma once

#include "Component.h"
#include "AssignableList.h"
#include "LookAndFeel.h"

class List : virtual public Component, public AssignableList
{
public:
    List(const ListData *newListData);
    virtual ~List() = default;

    void setColour(uint32_t newColour);
    void setIndex(uint16_t newIndex);

    virtual void onTouchMove(const TouchEvent &touchEvent) override;
    virtual void onTouchDown(const TouchEvent &touchEvent) override;
    virtual void onTouchUp(const TouchEvent &touchEvent) override;

    virtual void onPotChange(const PotEvent &potEvent) override;
    virtual void onPotTouchDown(const PotEvent &potEvent) override;
    virtual void onPotTouchUp(const PotEvent &potEvent) override;

    virtual void resized(void) override;
    virtual void paint(Graphics &g) override;

protected:
    uint32_t colour;
    uint16_t index;

private:
};
