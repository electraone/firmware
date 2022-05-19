#pragma once

#include <vector>
#include <functional>
#include "Graphics.h"
#include "TouchCallback.h"
#include "PotListener.h"
#include "Value.h"
#include "helpers.h"

class Window;

/**
 * @brief represents an essential User-interface object.
 */
class Component : public Rectangle, public TouchCallback, public PotListener
{
public:
    /** Constructor.
     *
     * Creates an empty instance of the component.
     *
     * @see setId, setName, setBounds,
     */
    Component();

    /** Constructor. */
    Component(Component *newParent);

    /** Construstor.
     *
     * Creates component and sets its name.
     *
     * @param newName a name of the conponent.
     */
    Component(const char *newName);

    /** Destructor. */
    virtual ~Component();

    /**
     * Sets component identifier.
     *
     * @param newId component identifier.
     *
     * @see getId, findChildById
     */
    void setId(uint16_t newId);

    /**
     * Returns component identifier.
     *
     * @return component identifier.
     *
     * @see setId, findChildById
     */
    uint16_t getId(void) const;

    /**
     * Sets name of the component.
     *
     * @param newName component name.
     *
     * @see getName
     */
    void setName(const char *newName);

    /**
     * Returns name of the component.
     *
     * @return component name.
     *
     * @see setName
     */
    const char *getName(void) const;

    /**
     * Sets graphic object position and size.
     *
     * The moved() and resized() callbacks are triggered.
     *
     * @param rect position and location given by Rectangle object.
     *
     * @see moved, resized
     */
    void setBounds(const Rectangle &rect);

    /**
     * Sets graphic object position and size
     *
     * The moved() and resized() callbacks are triggered.
     *
     * @param newX X position of the object.
     * @param newY Y position of the object.
     * @param newWidth width of the object.
     * @param newHeight height of the object.
     *
     * @see moved, resized
     */
    void setBounds(uint16_t newX,
                   uint16_t newY,
                   uint16_t newWidth,
                   uint16_t newHeight);

    /**
     * Sets graphic object position.
     *
     * The moved() callback is triggered.
     *
     * @param newX X position of the object.
     * @param newY Y position of the object.
     *
     * @see moved
     */
    void setPosition(uint16_t newX, uint16_t newY);

    /**
     * Sets graphic object size.
     *
     * The resized() callback is triggered.
     *
     * @param newWidth width of the object.
     * @param newHeight height of the object.
     *
     * @see resized
     */
    void setSize(uint16_t newWidth, uint16_t newHeight);

    /**
     * Sets the new X coordinate.
     *
     * The moved() callback is triggered.
     *
     * @see moved
     */
    void setX(uint16_t newX);

    /**
     * Sets the new Y coordinate.
     *
     * The moved() callback is triggered.
     *
     * @see moved
     */
    void setY(uint16_t newY);

    /**
     * Sets the new width of the rectangle.
     *
     * The resized() callback is triggered.
     *
     * @see resized
     */
    void setWidth(uint16_t newWidth);

    /**
     * Sets the new height of the rectangle.
     *
     * The resized() callback is triggered.
     *
     * @see resized
     */
    void setHeight(uint16_t newHeight);

    /**
     * Sets the position of the rectangle's centre.
     *
     * The moved() callback is triggered.
     *
     * @param newX a new X coordinate.
     * @param newY a new Y coordinate.
     *
     * @see moved
     */
    void setCentre(uint16_t newCentreX, uint16_t newCentreY);

    /**
     * Marks the object as visible.
     *
     * @param shouldBeVisible set to true to make object visible.
     *
     * @see isVisible
     */
    virtual void setVisible(bool shouldBeVisible);

    /**
     * Gets information if the object is visible.
     *
     * @return true, when object is visible.
     *
     * @see setVisible
     */
    bool isVisible(void) const;

    /**
     * @todo candidates to be removed from here
     */
    virtual void setActive(bool shouldBeActive);
    virtual bool isActive(void) const;
    virtual void setDimmed(bool shouldBeDimmed);
    virtual bool isDimmed(void) const;

    /**
     * Schedules repaint of the graphic object.
     */
    void repaint(void);

    /**
     * Marks graphic object as (re)painted. The function is used by the
     * internal tasks to manage graphic object repainting.
     */
    void painted(void);

    /** Gets components absolute X position on the screen.
     *
     * @see getScreenY, getX
     */
    uint16_t getScreenX(void) const;

    /** Gets components absolute Y position on the screen.
     *
     * @see getScreenX, getY
     */
    uint16_t getScreenY(void) const;

    /**
     * Paints graphic object.
     */
    virtual void paint(Graphics &g) = 0;

    /**
     * Callback to handle graphic object being moved
     */
    virtual void moved(void);

    /**
     * Callback to handle graphic object resizing.
     */
    virtual void resized(void);

    /**
     * Checks if given coordinates are inside the graphic object.
     *
     * @param x X position of the hit coordinates.
     * @param y Y position of the hit coordinates.
     *
     * @return true, when coordinates are inside the object.
     */
    virtual bool hitTest(uint16_t x, uint16_t y);

    /**
     * A callback to handle objects visibility changes.
     */
    virtual void visibilityChanged(void);

    /**
     * Checks whether or not the lower object is fully covered by the
     * upper object.
     *
     * @param upper pointer to the upper graphic object.
     * @param lower pointer to the lower graphic object.
     *
     * @return true, when any part of the lower object can be seen.
     */
    static bool canBeSeen(Component *upper, Component *lower);

    /**
     * Checks if the coordinates are inside given area.
     *
     * @param pointX X position of the coordinates to check.
     * @param pointY Y position of the coordinates to check.
     * @param areaX X position of the area.
     * @param areaY Y position of the area.
     * @param areaWidth width of the area.
     * @param areaHeight height of the area.
     *
     * @return true, when point coordinates are inside the area.
     */
    static bool isInside(uint16_t pointX,
                         uint16_t pointY,
                         uint16_t areaX,
                         uint16_t areaY,
                         uint16_t areaWidth,
                         uint16_t areaHeight);

    /**
     * Checks whether or not the first and second objects overlap.
     *
     * @param first the first object to check.
     * @param second, the second object to check.
     *
     * @return true, when the objects overlap.
     */
    static bool isOverlaping(Component *first, Component *second);

    /**
     * Returns the parent component.
     *
     * @return pointer to the component, or nullptr
     */
    Component *getParentComponent(void) const;

    /**
     * Returns the window containing the component.
     *
     * @return pointer to the window, or nullptr
     */
    Window *getWindow(void) const;

    /**
     * Adds a child component to this component. The component is not made
     * visible.
     *
     * @param component a new component to add.
     *
     * @see addAndMakeVisible
     */
    virtual void addChildComponent(Component *component);

    /**
     * Adds a child component to this component and makes it visible.
     *
     * @param component a new component to add.
     *
     * @see addChildComponent
     */
    virtual void addAndMakeVisible(Component *component);

    /**
     * Removes all component's children components. The child components are
     * not deleted. It is responsibility of the calling function to take care
     * of that.
     */
    void removeAllChildren(void);

    /**
     * Removes all component's children components and deletes them.
     */
    void deleteAllChildren(void);

    void paintWithChildren(Graphics &g);

    // Getters

    /**
     * Gets a list of all children components of the component.
     */
    const std::vector<Component *> &getChildren(void);

    /**
     * Gets component by the index of the list of children components.
     *
     * The index is actually a zIndex of the component layers.
     *
     * @see getChildComponent, getNumChildComponents
     */
    Component *getChildComponent(uint16_t index);

    /**
     * Gets component by the Id.
     *
     * @see setId, getId, getChildComponent
     */
    Component *findChildById(uint16_t id);

    /**
     * Gets the number of child components of the component.
     */
    int getNumChildComponents(void) const;

    /**
     * Gets local bounds of the component.
     *
     */
    Rectangle getLocalBounds(void) const;

protected:
    static const int MaxNameLength = 15;
    char name[MaxNameLength + 1];

    struct {
        uint16_t id : 9;
        bool visible : 1;
        bool active : 1;
        bool dimmed : 1;
    };

    Component *queueEntry;
    std::vector<Component *> components;

    void setParentComponent(Component *newParent);

private:
    Component *parentComponent;
    friend class Window;
    void repaintQueueItem(void);
    bool shouldBeDisplayed(void) const;
};
