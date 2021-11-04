#pragma once

#include "Component.h"
#include "AssignableList.h"
#include "LookAndFeel.h"
#include <vector>

class List : public Component, public AssignableList
{
public:
    List();
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
    void paintDots(Graphics &g);
    void paintBar(Graphics &g);
};
