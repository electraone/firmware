#include "Component.h"
#include "Window.h"
#include "helpers.h"

Component::Component()
    : id(0),
      parentComponent(nullptr),
      visible(true),
      active(false),
      queueEntry(nullptr)
{
    copyString(name, "", MaxNameLength);
}

Component::Component(Component *newParent)
    : id(0),
      parentComponent(newParent),
      visible(true),
      active(false),
      queueEntry(nullptr)
{
    copyString(name, "", MaxNameLength);
}

Component::Component(const char *newName)
    : id(0),
      parentComponent(nullptr),
      visible(true),
      active(false),
      queueEntry(nullptr)
{
    copyString(name, newName, MaxNameLength);
}

void Component::setId(uint16_t newId)
{
    id = newId;
}

uint16_t Component::getId(void) const
{
    return (id);
}

void Component::setName(const char *newName)
{
    copyString(name, newName, MaxNameLength);
}

const char *Component::getName(void) const
{
    return (name);
}

Component *Component::getParentComponent(void) const
{
    return (parentComponent);
}

// \todo this needs better implementation. It is not the right assumption
// that the top parent component is a window. ActiveWindow should be used
// instead.
Window *Component::getWindow(void) const
{
    Component *component = getParentComponent();
    Component *topComponent = component;

    while ((component = component->getParentComponent()) != nullptr) {
        topComponent = component;
    }

    return (static_cast<Window *>(topComponent));
}

void Component::setBounds(const Rectangle &rect)
{
    Rectangle::setBounds(rect);
    moved();
    resized();
}

void Component::setBounds(uint16_t newX,
                          uint16_t newY,
                          uint16_t newWidth,
                          uint16_t newHeight)
{
    Rectangle::setBounds(newX, newY, newWidth, newHeight);
    moved();
    resized();
}

void Component::setPosition(uint16_t newX, uint16_t newY)
{
    Rectangle::setPosition(newX, newY);
    moved();
}

void Component::setSize(uint16_t newWidth, uint16_t newHeight)
{
    Rectangle::setSize(newWidth, newHeight);
    resized();
}

void Component::setX(uint16_t newX)
{
    Rectangle::setX(newX);
    moved();
}

void Component::setY(uint16_t newY)
{
    Rectangle::setY(newY);
    moved();
}

void Component::setWidth(uint16_t newWidth)
{
    Rectangle::setWidth(newWidth);
    resized();
}

void Component::setHeight(uint16_t newHeight)
{
    Rectangle::setHeight(newHeight);
    resized();
}

void Component::setCentre(uint16_t newCentreX, uint16_t newCentreY)
{
    Rectangle::setCentre(newCentreX, newCentreY);
    moved();
}

void Component::setVisible(bool shouldBeVisible)
{
    bool chg = visible != shouldBeVisible;
    visible = shouldBeVisible;
    if (chg) {
        visibilityChanged();
    }
}

bool Component::isVisible(void) const
{
    return (visible);
}

void Component::setActive(bool shouldBeActive)
{
    active = shouldBeActive;
}

bool Component::isActive(void) const
{
    return (active);
}

void Component::repaint(void)
{
    if (isVisible()) {
        repaintQueueItem();
    }
}

void Component::painted(void)
{
    queueEntry = nullptr;
}

uint16_t Component::getScreenX(void) const
{
    if (!parentComponent) {
        return (getX());
    } else {
        return (parentComponent->getScreenX() + getX());
    }
}

uint16_t Component::getScreenY(void) const
{
    if (!parentComponent) {
        return (getY());
    } else {
        return (parentComponent->getScreenY() + getY());
    }
}

void Component::resized(void)
{
}

void Component::moved(void)
{
}

bool Component::hitTest(uint16_t x, uint16_t y)
{
    return (
        isInside(x, y, getScreenX(), getScreenY(), getWidth(), getHeight()));
}

void Component::visibilityChanged(void)
{
}

void Component::addChildComponent(Component *component)
{
    component->parentComponent = this;
    components.push_back(component);
}

void Component::addAndMakeVisible(Component *component)
{
    addChildComponent(component);
    component->setVisible(true);
}

void Component::removeAllChildren(void)
{
    while (!components.empty()) {
        components.pop_back();
    }

    components = std::vector<Component *>();
}

void Component::deleteAllChildren(void)
{
    while (!components.empty()) {
        const Component *component = components.back();
        components.pop_back();
        delete component;
    }

    components = std::vector<Component *>();
}

void Component::paintWithChildren(Graphics &g)
{
    if (isVisible()) {
        g.setActiveWindowPosition(getScreenX(), getScreenY());
        g.setActiveWindowSize(getWidth(), getHeight());
        paint(g);

        for (auto &component : components) {
            component->paintWithChildren(g);
        }
        //g.drawRect (0, 0, getWidth(), getHeight(), ElectraColours::rgb565NumericWhite);
    }
}

const std::vector<Component *> &Component::getChildren(void)
{
    return (components);
}

Component *Component::getChildComponent(uint16_t index)
{
    if (index < components.size()) {
        return (components[index]);
    }
    return (nullptr);
}

Component *Component::findChildById(uint16_t componentId)
{
    for (auto &component : components) {
        if (component->getId() == componentId) {
            return (component);
        }
    }

    return (nullptr);
}

int Component::getNumChildComponents(void) const
{
    return (components.size());
}

Rectangle Component::getLocalBounds(void) const
{
    return (Rectangle(0, 0, getWidth(), getHeight()));
}

void Component::setParentComponent(Component *newParent)
{
    parentComponent = newParent;
}

bool Component::canBeSeen(Component *upper, Component *lower)
{
    return ((lower->getX() < upper->getX()) || (lower->getY() < upper->getY())
            || ((lower->getX() + lower->getWidth())
                > (upper->getX() + upper->getWidth()))
            || ((lower->getY() + lower->getHeight())
                > (upper->getY() + upper->getHeight())));
}

bool Component::isInside(uint16_t pointX,
                         uint16_t pointY,
                         uint16_t areaX,
                         uint16_t areaY,
                         uint16_t areaWidth,
                         uint16_t areaHeight)
{
    return ((pointX > areaX) && (pointX < (areaX + areaWidth))
            && (pointY > areaY) && (pointY < (areaY + areaHeight)));
}

bool Component::isOverlaping(Component *first, Component *second)
{
    uint16_t l1x = first->getX();
    uint16_t l1y = first->getY();
    uint16_t r1x = first->getRight();
    uint16_t r1y = first->getBottom();
    uint16_t l2x = second->getX();
    uint16_t l2y = second->getY();
    uint16_t r2x = second->getRight();
    uint16_t r2y = second->getBottom();

    // If one rectangle is on left side of other
    if (l1x >= r2x || l2x >= r1x) {
        return (false);
    }

    // If one rectangle is above other
    if (l1y <= r2y || l2y <= r1y) {
        return (false);
    }

    return (true);
}

#include "BarHorizontal.h"
#include "List.h"

Component *Component::createComponent(uint8_t type)
{
    Component *c = nullptr;

    if (type == 1) {
        if ((c = new BarHorizontal) != nullptr) {
            logMessage("building fader");
        }
    } else {
        if ((c = new List) != nullptr) {
            logMessage("building list");
        }
    }

    if (c) {
    }

    return (c);
}

// \todo Move this to an appropriate place
#include <CircularBuffer.h>
extern CircularBuffer<Component *, 100> repaintQueue;

void Component::repaintQueueItem(void)
{
#ifdef DEBUG
    logMessage("repaint request '%s' [%d %d %d %d] size=%d",
               getName(),
               this->getX(),
               this->getY(),
               this->getWidth(),
               this->getHeight(),
               repaintQueue.size());
#endif

    if (queueEntry == nullptr) {
        repaintQueue.push(this);
        queueEntry = repaintQueue.last();
    } else {
        if (!repaintQueue.isEmpty()) {
            if (repaintQueue.first() == this) {
                repaintQueue.shift();
            }
        }

        repaintQueue.push(this);
        queueEntry = repaintQueue.last();
    }
}
