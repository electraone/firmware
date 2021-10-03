#pragma once

class Component;

class PotEvent
{
public:
    PotEvent()
    {
    }

    PotEvent(uint8_t newPotId) : potId(newPotId), relativeChange(0)
    {
    }

    PotEvent(uint8_t newPotId, int16_t newRelativeChange)
        : potId(newPotId), relativeChange(newRelativeChange)
    {
    }

    uint8_t getPotId(void) const
    {
        return (potId);
    }

    int16_t getRelativeChange(void) const
    {
        return (relativeChange);
    }

    Component *getEventComponent(void) const
    {
        return (eventComponent);
    }

    void setEventCompoment(Component *component)
    {
        eventComponent = component;
    }

private:
    uint8_t potId;
    int16_t relativeChange;
    Component *eventComponent;
};
