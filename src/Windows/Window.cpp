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

/**
 * @file Window.cpp
 *
 * @brief A base class for implementing Window objects to process
 *  users interactions.
 */

#include "Window.h"
#include "System.h"

Window::Window()
    : activeComponent{},
      parentWindow(nullptr),
      active(false),
      uiEventsValid(true),
      contentComponent(nullptr),
      numActivePotTouch(0),
      potTouchComponents{ nullptr }

{
    setBounds(0, 22, 1024, 560); // default window position and size
    setVisible(true);
    System::windowManager.addWindow(this);
}

Window::Window(uint16_t newX,
               uint16_t newY,
               uint16_t newWidth,
               uint16_t newHeight)
    : activeComponent{},
      parentWindow(nullptr),
      active(false),
      uiEventsValid(true),
      contentComponent(nullptr),
      numActivePotTouch(0),
      potTouchComponents{ nullptr }
{
    setVisible(true);
    setBounds(newX, newY, newWidth, newHeight);
    System::windowManager.addWindow(this);
}

Window::~Window()
{
    buttonBroadcaster.removeListener(this);
    System::tasks.clearRepaintGraphics();
    System::windowManager.removeWindow(this);

    if (contentComponent) {
        delete contentComponent;
    }
}

Component *Window::checkHit(Component *component, uint16_t x, uint16_t y)
{
    Component *hitComponent = component;

    for (auto &c : component->getChildren()) {
        if (c->isVisible()) {
            if (c->hitTest(x, y)) {
                hitComponent = checkHit(c, x, y);
            }
        }
    }
    return (hitComponent);
}

std::vector<TouchEvent> Window::processTouch(const TouchPoint &touchPoint)
{
    std::vector<TouchEvent> touchEvents;
    TouchEvent touchEvent(touchPoint);

    Component *component =
        checkHit(contentComponent, touchPoint.x, touchPoint.y);
    touchEvent.setEventCompoment(component);
    touchEvents.push_back(touchEvent);

    return (touchEvents);
}

void Window::resetActiveTouch(void)
{
#ifdef TODO // \todo why this is disabled?
    for (auto &component : contentComponent->getChildren()) {
        //        component->repaint();
    }
#endif
    for (uint8_t i = 0; i < numActiveComponents; i++) {
        activeComponent[i] = nullptr;
    }
    resetAllActivePotComponents();
}

void Window::setActiveComponent(Component *component, uint8_t touchId)
{
    if (component) {
        activeComponent[touchId] = component;
    }
}

void Window::resetActiveComponent(uint8_t touchId)
{
    activeComponent[touchId] = nullptr;
}

Component *Window::getActiveComponent(uint8_t touchId)
{
    return activeComponent[touchId];
}

void Window::paint([[maybe_unused]] Graphics &g)
{
}

void Window::setVisible(bool shouldBeVisible)
{
    Component *c = getOwnedContent();

    Component::setVisible(shouldBeVisible);

    if (c) {
        c->setVisible(shouldBeVisible);
    }
    if (!shouldBeVisible) {
        System::windowManager.repaintAll();
    }
}

void Window::onTouchOutside(void)
{
}

void Window::addChildComponent([[maybe_unused]] Component *component)
{
    System::logger.write(
        LOG_ERROR,
        "window.addChildComponent() do use it. Use setOwnedContent()");
}

void Window::addAndMakeVisible([[maybe_unused]] Component *component)
{
    System::logger.write(
        LOG_ERROR,
        "window.addChildComponent() do use it. Use setOwnedContent()");
}

void Window::setOwnedContent(Component *newComponent)
{
    contentComponent = newComponent;
    newComponent->setParentComponent(this);
}

Component *Window::getOwnedContent(void)
{
    return (contentComponent);
}

Component *Window::replaceOwnedContent(Component *newComponent)
{
    if (newComponent) {
        setOwnedContent(newComponent);
        newComponent->setVisible(true);
    }
    repaint();

    uiEventsValid = false;

    return (newComponent);
}

void Window::resetAllActivePotComponents(void)
{
    for (uint8_t i = 0; i < 12; i++) {
        potTouchComponents[i] = nullptr;
    }
    numActivePotTouch = 0;
}

Component *Window::getActivePotComponent(void) const
{
    for (uint8_t i = 0; i < 12; i++) {
        if (potTouchComponents[i]) {
            return (potTouchComponents[i]);
        }
    }
    return (nullptr);
}

uint8_t Window::getNumActivePotTouch(void)
{
    return (numActivePotTouch);
}

void Window::setActivePotTouch(uint8_t potId, Component *component)
{
    potTouchComponents[potId] = component;
    numActivePotTouch++;
}

void Window::resetActivePotTouch(uint8_t potId)
{
    potTouchComponents[potId] = nullptr;
    if (numActivePotTouch > 0) {
        numActivePotTouch--;
    }
}

void Window::setParentWindow(Window *newParentWindow)
{
    parentWindow = newParentWindow;
}

Window *Window::getParentWindow(void) const
{
    return (parentWindow);
}

void Window::setActive(bool shouldBeActive)
{
    active = shouldBeActive;
}

bool Window::isActive(void) const
{
    return (active);
}

void Window::setUiEventsValid(void)
{
    uiEventsValid = true;
}

bool Window::areUiEventsValid(void) const
{
    return (uiEventsValid);
}

void Window::close(Window *window)
{
    if (window) {
        delete window;
    }
}
