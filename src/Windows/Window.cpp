#include "Window.h"
#include "System.h"

Window::Window() : activeComponent{}, contentComponent(nullptr)

{
    setBounds(0, 22, 1024, 560);
    System::windowManager.addWindow(this);
}

Window::Window(uint16_t newX,
               uint16_t newY,
               uint16_t newWidth,
               uint16_t newHeight)
    : activeComponent{}, contentComponent(nullptr)
{
    setBounds(newX, newY, newWidth, newHeight);
    System::windowManager.addWindow(this);
}

Window::~Window()
{
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

    if (touchPoint.event == TouchPoint::End) {
        activeComponent[id] = nullptr;
    } else {
        if (component) {
            activeComponent[id] = component;
        }
    }

    return (touchEvents);
}

void Window::resetActiveTouch(void)
{
    for (auto &component : contentComponent->getChildren()) {
        component->setActive(false);
        component->repaint();
    }
    for (uint8_t i = 0; i < numActiveComponents; i++) {
        activeComponent[i] = nullptr;
    }
}

void Window::setActiveComponent(uint16_t componentId)
{
    for (auto &component : contentComponent->getChildren()) {
        component->setActive((component->getId() == componentId) ? true
                                                                 : false);
    }
}

Component *Window::getActiveComponent(void)
{
    for (auto &component : contentComponent->getChildren()) {
        if (component->isActive()) {
            return (component);
        }
    }
    return (nullptr);
}

void Window::paint(Graphics &g)
{
    if (isVisible()) {
        contentComponent->paintWithChildren(g);
    }
}

void Window::setVisible(bool shouldBeVisible)
{
    Component *c = getOwnedContent();

    if (c) {
        c->setVisible(shouldBeVisible);
    }

    return (Component::setVisible(shouldBeVisible));
}

void Window::display(void)
{
    repaint();
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
