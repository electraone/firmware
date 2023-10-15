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
 * @file Window.h
 *
 * @brief A base class for implementing Window objects to process
 *  users interactions.
 */

#pragma once

#include "ButtonListener.h"
#include "Component.h"
#include "TouchEvent.h"
#include <vector>

class Window : public Component, public ButtonListener
{
public:
    Window();
    Window(uint16_t x, uint16_t y, uint16_t width, uint16_t height);
    virtual ~Window();

    // LCD touch management
    virtual std::vector<TouchEvent> processTouch(const TouchPoint &touchPoint);
    void resetActiveTouch(void);
    void setActiveComponent(Component *component, uint8_t touchId);
    void resetActiveComponent(uint8_t touchId);
    Component *getActiveComponent(uint8_t touchId);

    // Paint releated methods
    virtual void paint(Graphics &g) override;
    void setVisible(bool shouldBeVisible);

    // Event processing
    void setActive(bool shouldBeActive);
    bool isActive(void) const;
    void setUiEventsValid(void);
    bool areUiEventsValid(void) const;
    virtual void onTouchOutside(void);

    void addChildComponent(Component *component) override;
    void addAndMakeVisible(Component *component) override;

    void setOwnedContent(Component *newComponent);
    Component *getOwnedContent(void);
    Component *replaceOwnedContent(Component *newComponent);

    void setParentWindow(Window *parentWindow);
    Window *getParentWindow(void) const;

    void setActivePotTouch(uint8_t potId, Component *component);
    void resetActivePotTouch(uint8_t potId);
    uint8_t getNumActivePotTouch(void);
    Component *getActivePotComponent(void) const;
    void resetAllActivePotComponents(void);
    static void close(Window *window);

private:
    static Component *checkHit(Component *component, uint16_t x, uint16_t y);

    static const uint8_t numActiveComponents = 5;

    Component *activeComponent[numActiveComponents];
    Window *parentWindow;
    bool active;
    bool uiEventsValid;

protected:
    Component *contentComponent;

    // Ui state
    uint8_t numActivePotTouch;
    Component *potTouchComponents[12];
};
