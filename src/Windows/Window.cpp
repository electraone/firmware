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
    for (auto &component : contentComponent->getChildren()) {
        component->repaint();
    }
    for (uint8_t i = 0; i < numActiveComponents; i++) {
        activeComponent[i] = nullptr;
    }
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

void Window::paint(Graphics &g)
{
    if (isVisible() && contentComponent) {
        contentComponent->paintWithChildren(g);
    }
}

void Window::setVisible(bool shouldBeVisible)
{
    Component *c = getOwnedContent();

    if (c) {
        c->setVisible(shouldBeVisible);
    }

    Component::setVisible(shouldBeVisible);

    if (!shouldBeVisible) {
        System::windowManager.repaintAll();
    }
}

void Window::onTouchOutside(void)
{
}

void Window::addChildComponent(Component *component)
{
    logMessage("window.addChildComponent() do use it. Use setOwnedContent()");
}

void Window::addAndMakeVisible(Component *component)
{
    logMessage("window.addChildComponent() do use it. Use setOwnedContent()");
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
    auto originalComponent = getOwnedContent();

    if (originalComponent) {
        delete originalComponent;
    }
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
    numActivePotTouch--;
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
