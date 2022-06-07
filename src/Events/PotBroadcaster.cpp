#include "PotBroadcaster.h"
#include "PotListener.h"
#include "Component.h"
#include "System.h"

PotBroadcaster::PotBroadcaster() : iteratorInvalidated(false)
{
}

void PotBroadcaster::addListener(PotListener *newListener)
{
    listeners[newListener] = 1;
}

void PotBroadcaster::removeListener(PotListener *listenerToRemove)
{
    iteratorInvalidated = true;
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
    Window *originatingWindow = System::windowManager.getActiveWindow();

    iteratorInvalidated = false;
    auto i = listeners.begin();

    while (i != listeners.end()) {
        auto current = i++;
        auto listener = current->first;

        if ((listener->getPotId() == PotListener::AllPots)
            || (potEvent.getPotId() == listener->getPotId())) {
            Component *eventComponent = static_cast<Component *>(listener);
            potEvent.setEventCompoment(eventComponent);

            if (listener->getNumValues() > 0) {
                potEvent.setAcceleratedChange(
                    listener->computeRate(potEvent.getRelativeChange()));
            } else {
                potEvent.setAcceleratedChange(potEvent.getRelativeChange());
            }

            if (eventComponent->getWindow() == originatingWindow) {
                for (Component *c = eventComponent; c;
                     c = c->getParentComponent()) {
                    if (PotListener *pl = dynamic_cast<PotListener *>(c)) {
                        (pl->*function)(potEvent);

                        if (function == &PotListener::onPotTouchUp) {
                            (pl->resetStepCount)();
                        }
                    }

                    if (originatingWindow
                        != System::windowManager.getActiveWindow()) {
                        break;
                    }
                }
            }
        }
        if (iteratorInvalidated) {
            break;
        }
    }
}
