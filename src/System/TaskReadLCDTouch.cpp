#include "TaskReadLCDTouch.h"
#include "Hardware.h"
#include "App.h"

/*
 * Read LCD capacitive touch
 */
void readLCDTouch(void)
{
    static Component *activeComponent[NUM_TOUCH_POINTS];

    //logMessage ("readLCDTouch");

    if (Hardware::touch.readTouch() == 0) {
        return;
    }

    Window *originatingWindow = System::windowManager.getActiveWindow();

    while (Hardware::touch.eventsAvailable()) {
        TouchPoint touchPoint = Hardware::touch.readEvents();

        if (touchPoint.event != TouchPoint::None) {
            std::vector<TouchEvent> touchEvents =
                originatingWindow->processTouch(touchPoint);

            for (auto &touchEvent : touchEvents) {
                Component *eventComponent = nullptr;
                uint8_t id = touchEvent.getId();

                if (activeComponent[id]) {
                    eventComponent = activeComponent[id];
                    touchEvent.setEventCompoment(eventComponent);
                } else {
                    eventComponent = touchEvent.getEventComponent();
                    activeComponent[id] = eventComponent;
                }

                triggerComponentCallbacks(eventComponent, touchEvent);

                if (touchEvent.getEvent() == TouchPoint::End) {
                    activeComponent[id] = nullptr;
                }
            }
        }
    }
}

void triggerComponentCallbacks(Component *component,
                               const TouchEvent &touchEvent)
{
    TouchPoint::Event state = touchEvent.getEvent();

    for (Component *c = component; c; c = c->getParentComponent()) {
        if (state == TouchPoint::Start) {
            c->onTouchDown(touchEvent);
        } else if (state == TouchPoint::Move) {
            c->onTouchMove(touchEvent);
        } else if (state == TouchPoint::End) {
            c->onTouchUp(touchEvent);
        } else if (state == TouchPoint::LongHold) {
            c->onTouchLongHold(touchEvent);
        } else if (state == TouchPoint::Click) {
            c->onTouchClick(touchEvent);
        } else if (state == TouchPoint::DoubleClick) {
            c->onTouchDoubleClick(touchEvent);
        }
    }
}
