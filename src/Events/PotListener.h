#pragma once

#include <cstdint>
#include "PotBroadcaster.h"
#include "PotEvent.h"

typedef uint8_t handle_t;

/**
 * @brief provides callbacks for handling Pot events.
 */
class PotListener
{
public:
    /**
    * Constructor.
    */
    PotListener() = default;

    /**
     * Destructor
     */
    virtual ~PotListener() = default;

    /**
     * Called when the potentiometer is turned.
     *
     * @param details about the pot activity, including information
     *        about the component assigned to the pot
     */
    virtual void onPotChange(const PotEvent &potEvent)
    {
    }

    /**
     * Called when the potentiometer is touched with a finger.
     *
     * @param details about the pot activity, including information
     *        about the component assigned to the pot
     */
    virtual void onPotTouchDown(const PotEvent &potEvent)
    {
    }

    /**
     * Called when a finger is lifted of the potentiometer.
     *
     * @param details about the pot activity, including information
     *        about the component assigned to the pot
     */
    virtual void onPotTouchUp(const PotEvent &potEvent)
    {
    }

    /**
     * Assigns the pot to the listener object.
     *
     * @param an identifier of the pot. It is a number in range 0 to 11, or
     *        AllPots constant for catching events from all pots.
     */
    void assignPot(uint8_t newPotId = AllPots)
    {
        potId = newPotId;
        potBroadcaster.addListener(this);
    }

    /**
     * Assigns all pots to the listener object.
     */
    void assignAllPots(void)
    {
        potId = AllPots;
        potBroadcaster.addListener(this);
    }

    /**
     * Stops listening for assigned pots.
     */
    void releasePot(void)
    {
        potBroadcaster.removeListener(this);
    }

    static constexpr uint8_t AllPots = 255;
    static PotBroadcaster potBroadcaster;
    uint8_t potId;
};
