#include "TouchEvent.h"
#include "Component.h"

TouchEvent::TouchEvent(const TouchPoint &touchPoint)
    : TouchPoint(touchPoint), eventComponent(nullptr)
{
}

uint8_t TouchEvent::getId() const
{
    return (id);
}

TouchPoint::Event TouchEvent::getEvent() const
{
    return (event);
}

uint16_t TouchEvent::getScreenX(void) const
{
    return (x);
}

uint16_t TouchEvent::getScreenY(void) const
{
    return (y);
}

uint16_t TouchEvent::getX(void) const
{
    if (eventComponent) {
        return (constrain(
            x - eventComponent->getScreenX(), 0, eventComponent->getWidth()));
    }

    return x;
}

uint16_t TouchEvent::getY(void) const
{
    if (eventComponent) {
        return (constrain(
            y - eventComponent->getScreenY(), 0, eventComponent->getHeight()));
    }

    return y;
}

uint16_t TouchEvent::getTouchDownScreenX(void) const
{
    return (xStart);
}

uint16_t TouchEvent::getTouchDownScreenY(void) const
{
    return (yStart);
}

uint16_t TouchEvent::getTouchDownX(void) const
{
    if (eventComponent) {
        return (constrain(xStart - eventComponent->getScreenX(),
                          0,
                          eventComponent->getWidth()));
    }

    return x;
}

uint16_t TouchEvent::getTouchDownY(void) const
{
    if (eventComponent) {
        return (constrain(yStart - eventComponent->getScreenY(),
                          0,
                          eventComponent->getHeight()));
    }

    return y;
}

Component *TouchEvent::getEventComponent(void) const
{
    return (eventComponent);
}

void TouchEvent::setEventCompoment(Component *component)
{
    eventComponent = component;
}
