#pragma once

class Value
{
public:
    Value() : absolute(0.0f), min(0.0f), max(1.0f), relative(0.0f)
    {
    }

    Value(float newAbsolute, float newMin, float newMax)
        : absolute(newAbsolute), min(newMin), max(newMax)
    {
        calculateRelative();
    }

    void set(float newAbsolute)
    {
        absolute = constrain(newAbsolute, min, max);
        calculateRelative();
    }

    void apply(float delta)
    {
        set(absolute + (delta * getStep()));
    }

    void setMin(float newMin)
    {
        min = newMin;
        calculateRelative();
    }

    void setMax(float newMax)
    {
        max = newMax;
        calculateRelative();
    }

    void setRange(float newMin, float newMax)
    {
        min = newMin;
        max = newMax;
        calculateRelative();
    }

    float getRelative(void) const
    {
        return (relative);
    }

    float get(void) const
    {
        return (absolute);
    }

    float getMin(void) const
    {
        return (min);
    }

    float getMax(void) const
    {
        return (max);
    }

    float getStep(void) const
    {
        return ((max - min) / 128.0f); // \todo fixed resolution for now
    }

private:
    void calculateRelative(void)
    {
        relative = map(absolute, min, max, min * (1.0f / max), 1.0f);
    }

    float absolute;
    float min;
    float max;
    float relative;
};
