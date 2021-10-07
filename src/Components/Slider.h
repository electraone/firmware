#pragma once

#include "Component.h"

class Slider : public Component
{
public:
    enum ValueBoxPosition {
        NoValueBox,
        ValueBoxLeft,
        ValueBoxRight,
        ValueBoxAbove,
        ValueBoxBelow
    };

    Slider();
    virtual ~Slider() override = default;

    void setValueBoxPosition(ValueBoxPosition newValueBoxPosition);
    ValueBoxPosition getValueBoxPosition(void) const;
    void setMinimum(int16_t newMin);
    int16_t getMinimum(void) const;
    void setMaximum(int16_t newMax);
    int16_t getMaximum(void) const;
    void setRange(int16_t newMinimum, int16_t newMaximum);
    void setValue(int16_t newValue);
    int16_t getValue(int16_t value) const;
    void setValueFormat(const char *newValueFormat);

    virtual void resized(void) override;
    virtual void paint(Graphics &g) override = 0;

    virtual void onTouchMove(const TouchEvent &touchEvent) override;
    virtual void onTouchDown(const TouchEvent &touchEvent) override;
    virtual void onTouchUp(const TouchEvent &touchEvent) override;

    std::function<void(int16_t value)> onValueChange;
    std::function<void(int16_t value)> onDragStart;
    std::function<void(int16_t value)> onDragEnd;

protected:
    virtual void paintValueBox(Graphics &g);

    int16_t min;
    int16_t max;
    int16_t value;

    const char *formatString;
    ValueBoxPosition valueBoxPosition;
};
