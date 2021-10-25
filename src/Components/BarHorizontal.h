#pragma once

#include "Slider.h"
#include "LookAndFeel.h"

class BarHorizontal : public Slider
{
public:
    BarHorizontal();
    ~BarHorizontal() override = default;

    virtual void onTouchMove(const TouchEvent &touchEvent) override;
    virtual void onTouchDown(const TouchEvent &touchEvent) override;
    virtual void onTouchUp(const TouchEvent &touchEvent) override;

    void paint(Graphics &g) override;

private:
};
