/*
* Electra One MIDI Controller Firmware
* See COPYRIGHT file at the top of the source tree.
*
* This product includes software developed by the
* Electra One Project (http://electra.one/).
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.
*/

#include "TaskReadLCDTouch.h"
#include "Hardware.h"
#include "App.h"

/*
 * Read LCD capacitive touch
 */
void readLCDTouch(void)
{
    //logMessage ("readLCDTouch");

    static Window *previousWindow = System::windowManager.getActiveWindow();
    static bool waitingForStart = false;

    if (Hardware::touch.readTouch() == 0) {
        return;
    }

    Window *originatingWindow = System::windowManager.getActiveWindow();

    // Detect a window change or window content change.
    // All events will be thrown away till next start
    // comes in.
    if ((originatingWindow != previousWindow)
        || !originatingWindow->areUiEventsValid()) {
        waitingForStart = true;
    }

    previousWindow = originatingWindow;

#ifdef DEBUG
    System::logger.write(
        ERROR,
        "received touch from active window: address=%x, window=%s",
        originatingWindow,
        originatingWindow->getName());
#endif

    while ((originatingWindow == System::windowManager.getActiveWindow())
           && Hardware::touch.eventsAvailable()) {
        TouchPoint touchPoint = Hardware::touch.readEvents();

        if (waitingForStart && touchPoint.event != TouchPoint::Start) {
            continue;
        }

        if (touchPoint.event != TouchPoint::None) {
            if (touchPoint.event == TouchPoint::Start) {
                waitingForStart = false;
                originatingWindow->setUiEventsValid();
                if (!Component::isInside(touchPoint.xStart,
                                         touchPoint.yStart,
                                         originatingWindow->getX(),
                                         originatingWindow->getY(),
                                         originatingWindow->getWidth(),
                                         originatingWindow->getHeight())) {
                    originatingWindow->onTouchOutside();
                    return;
                }
            }

            std::vector<TouchEvent> touchEvents =
                originatingWindow->processTouch(touchPoint);

            for (auto &touchEvent : touchEvents) {
                Component *eventComponent = nullptr;
                uint8_t id = touchEvent.getId();

                if (originatingWindow->getActiveComponent(id)) {
                    eventComponent = originatingWindow->getActiveComponent(id);
                    touchEvent.setEventCompoment(eventComponent);
                } else {
                    eventComponent = touchEvent.getEventComponent();
                    originatingWindow->setActiveComponent(eventComponent, id);
                }

                triggerComponentCallbacks(
                    originatingWindow, eventComponent, touchEvent);
                if (originatingWindow != System::windowManager.getActiveWindow()
                    || !originatingWindow->areUiEventsValid()) {
                    break;
                }
                if (touchEvent.getEvent() == TouchPoint::End) {
                    originatingWindow->resetActiveComponent(id);
                }
            }
        }
    }
}

void triggerComponentCallbacks(Window *window,
                               Component *component,
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

        if (window != System::windowManager.getActiveWindow()
            || !window->areUiEventsValid()) {
            break;
        }
    }
}
