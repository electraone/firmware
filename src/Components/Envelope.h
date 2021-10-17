#pragma once

#include "Component.h"

class Envelope : public Component
{
public:
    Envelope();
    virtual ~Envelope() = default;
    void setColour(uint32_t newColour);

    virtual void setValue(uint8_t handle, float newValue);
    virtual void setMin(uint8_t handle, float newMin);
    virtual void setMax(uint8_t handle, float newMax);

    void setActiveSegment(uint8_t newActiveSegment);
    void useDelay(bool shouldBeUsed);

    virtual void onTouchMove(const TouchEvent &touchEvent) override;
    virtual void onTouchDown(const TouchEvent &touchEvent) override;
    virtual void onTouchUp(const TouchEvent &touchEvent) override;

    virtual void onPotChange(const PotEvent &potEvent) override;
    virtual void onPotTouchDown(const PotEvent &potEvent) override;
    virtual void onPotTouchUp(const PotEvent &potEvent) override;

    virtual void computePoints(void) = 0;
    virtual void resized(void) override;
    virtual void paint(Graphics &g) override;

    static constexpr uint8_t delay = 0;

protected:
    uint16_t getSegmentWidth(uint8_t numSegments);

    std::vector<Point> points;
    std::vector<Value> values;
    float boundaryMin;
    float boundaryMax;
    int16_t baselineY;
    bool delayEnabled;

private:
    void paintContour(Graphics &g);
    void paintBaseline(Graphics &g);
    void paintMarkers(Graphics &g);
    void paintFills(Graphics &g);

    static bool findIntersection(uint16_t lineY,
                                 Point &C,
                                 Point &D,
                                 Point &intersection);

    uint32_t colour;
    uint8_t activeSegment;
};
