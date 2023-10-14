#pragma once

#include "Component.h"

class Slider : virtual public Component
{
public:
    enum ValueBoxPosition {
        NoValueBox,
        ValueBoxLeft,
        ValueBoxRight,
        ValueBoxAbove,
        ValueBoxBelow,
        ValueBoxCentre
    };

    Slider();
    virtual ~Slider() override = default;

    void setValueBoxPosition(ValueBoxPosition newValueBoxPosition);
    ValueBoxPosition getValueBoxPosition(void) const;
    void setMinimum(int16_t newMin);
    int16_t getMinimum(void) const;
    void setMaximum(int16_t newMax);
    int16_t getMaximum(void) const;
    void setRange(int16_t newMin, int16_t newMax);
    void setValue(int16_t newValue);
    void applyValue(int16_t delta);
    int16_t getValue(void) const;
    void setValueFormat(const char *newValueFormat);
    void setColour(uint32_t newColour);

    virtual void resized(void) override;
    virtual void paint(Graphics &g) override = 0;

    virtual void onTouchMove(const TouchEvent &touchEvent) override;
    virtual void onTouchDown(const TouchEvent &touchEvent) override;
    virtual void onTouchUp(const TouchEvent &touchEvent) override;

    virtual void onPotChange(const PotEvent &potEvent) override;
    virtual void onPotTouchDown(const PotEvent &potEvent) override;
    virtual void onPotTouchUp(const PotEvent &potEvent) override;

    std::function<void(int16_t value)> onValueChange;

    /* \todo commented out on purpose */
    std::function<void(int16_t value)> onDragStart;
    std::function<void(int16_t value)> onDragEnd;

protected:
    virtual void paintValueBox(Graphics &g);

    Value value;
    uint32_t colour;

    const char *formatString;
    ValueBoxPosition valueBoxPosition;
};
