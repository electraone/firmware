#include "PotBroadcaster.h"
#include "PotListener.h"
#include "Component.h"

PotBroadcaster::PotBroadcaster()
{
}

void PotBroadcaster::addListener(PotListener *newListener)
{
    listeners[newListener] = 1;
}

void PotBroadcaster::removeListener(PotListener *listenerToRemove)
{
    listeners.erase(listenerToRemove);
}

void PotBroadcaster::potChange(uint8_t potId, int16_t relativeChange)
{
    PotEvent potEvent(potId, relativeChange);
    triggerCallbacks(potEvent, &PotListener::onPotChange);
}

void PotBroadcaster::potTouchDown(uint8_t potId)
{
    PotEvent potEvent(potId);
    triggerCallbacks(potEvent, &PotListener::onPotTouchDown);
}

void PotBroadcaster::potTouchUp(uint8_t potId)
{
    PotEvent potEvent(potId);
    triggerCallbacks(potEvent, &PotListener::onPotTouchUp);
}

void PotBroadcaster::triggerCallbacks(PotEvent &potEvent, plFunction function)
{
    for (auto i = listeners.begin(); i != listeners.end(); i++) {
        auto listener = i->first;

        if ((listener->potId == PotListener::AllPots)
            || (potEvent.getPotId() == listener->potId)) {
            Component *eventComponent = static_cast<Component *>(listener);
            potEvent.setEventCompoment(eventComponent);

            for (Component *c = eventComponent; c;
                 c = c->getParentComponent()) {
                (static_cast<PotListener *>(c)->*function)(potEvent);
            }
        }
    }
}
