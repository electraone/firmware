#pragma once

#include "Slider.h"

class SliderVertical : public Slider
{
public:
    SliderVertical();
    ~SliderVertical() override = default;

    virtual void onTouchMove(const TouchEvent &touchEvent) override;
    virtual void onTouchDown(const TouchEvent &touchEvent) override;
    virtual void onTouchUp(const TouchEvent &touchEvent) override;

    void paint(Graphics &g) override;

private:
    static constexpr uint8_t indicatorSize = 22;
    static constexpr uint8_t indicatorRadius = indicatorSize / 2;
};
