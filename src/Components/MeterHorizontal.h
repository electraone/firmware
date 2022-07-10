#pragma once

#include "Slider.h"
#include "LookAndFeel.h"

class MeterHorizontal : public Slider
{
public:
    MeterHorizontal();
    virtual ~MeterHorizontal() override = default;

    void setColourInactive(uint32_t newColour);
    void setColourBackground(uint32_t newColour);

    virtual void onTouchMove(const TouchEvent &touchEvent) override;
    virtual void onTouchDown(const TouchEvent &touchEvent) override;
    virtual void onTouchUp(const TouchEvent &touchEvent) override;

    void paint(Graphics &g) override;

private:
    uint32_t colourInactive;
    uint32_t colourBackground;
};
