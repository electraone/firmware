#pragma once

#include <vector>
#include "TouchEvent.h"
#include "Component.h"
#include "ButtonListener.h"

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
    void paint(Graphics &g) override;
    void setVisible(bool shouldBeVisible) override;

    // Event processing
    void setActive(bool shouldBeActive);
    bool getActive(void) const;
    virtual void onTouchOutside(void);

    void addChildComponent(Component *component) override;
    void addAndMakeVisible(Component *component) override;

    void setOwnedContent(Component *newComponent);
    Component *getOwnedContent(void);

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

protected:
    Component *contentComponent;

    // Ui state
    uint8_t numActivePotTouch;
    Component *potTouchComponents[12];
};
