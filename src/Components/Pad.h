#pragma once

#include "Component.h"
#include "LookAndFeel.h"

class Pad : public Component
{
public:
    Pad();
    virtual ~Pad() = default;
    void setColour(uint32_t newColour);
    void setState(bool newState);
    bool getState(void) const;
    bool isDown(void) const;

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
    bool state;
};
