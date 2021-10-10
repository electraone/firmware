#pragma once

#include "Slider.h"

class Knob : public Slider
{
public:
    explicit Knob();
    virtual ~Knob() override = default;

    virtual void onTouchMove(const TouchEvent &touchEvent) override;
    virtual void onTouchDown(const TouchEvent &touchEvent) override;
    virtual void onTouchUp(const TouchEvent &touchEvent) override;

    void paint(Graphics &g) override;

private:
    void paintValue(Graphics &g,
                    uint16_t xCentre,
                    uint16_t yCentre,
                    uint16_t radius,
                    float angleZero,
                    float angle);

    void drawCurve(Graphics &g,
                   uint16_t xCentre,
                   uint16_t yCentre,
                   uint16_t radius,
                   float angleStart,
                   float angleEnd);

    void maskUnused(Graphics &g,
                    uint16_t xCentre,
                    uint16_t yCentre,
                    uint8_t quadrant,
                    uint8_t quadrantStart,
                    uint8_t quadrantEnd);

    static void maskCurveSeqment(Graphics &g,
                                 uint16_t x1,
                                 uint16_t y1,
                                 uint16_t x2,
                                 uint16_t y2,
                                 uint16_t x3,
                                 uint16_t y3,
                                 uint16_t x4,
                                 uint16_t y4);
    static uint8_t getQuadrant(float angle);

    static constexpr float angleTrackStart = 2.007f;
    static constexpr float angleTrackEnd = 7.418f;
};
