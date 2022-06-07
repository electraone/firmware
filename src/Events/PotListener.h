#pragma once

#include <cstdint>
#include <cstdlib>
#include <math.h>
#include "core_pins.h"
#include "wiring.h"
#include "PotBroadcaster.h"
#include "PotEvent.h"

/**
 * @brief provides callbacks for handling Pot events.
 */
class PotListener
{
public:
    /**
    * Constructor.
    */
    PotListener();

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
	 * @param a number of distict values within the pot range.
     */
    void assignPot(uint8_t newPotId = AllPots, uint16_t newNumValues = 0);

    /**
     * Assigns all pots to the listener object.
	 *
	 * @param a number of distict values within the pot range.
     */
    void assignAllPots(uint16_t newNumValues = 0);

    /**
     * Enable an encoder mode.
	 *
	 * @param true enables encoder style value reading, false keep the analog
     * pot reading.
     */
    void enableEncoderMode(bool newEncoderMode);

    /**
     * Stops listening for assigned pots.
     */
    void releasePot(void);

    /**
	 * Returns currently assigned pot.
	 */
    uint8_t getPotId(void);

    /**
	 * Returns number of distict values that the listener uses.
	 */
    uint16_t getNumValues(void);

    /**
	 * Calculates accelerated rate of change of the pot value
	 */
    int16_t computeRate(int16_t relativeChange);

    void resetStepCount(void);

    static constexpr uint8_t AllPots = 255;
    static PotBroadcaster potBroadcaster;

private:
    int16_t decreaseRate(int16_t relativeChange);
    static uint8_t getAccelerationTable(uint16_t numValues);
    static uint8_t getIntervalBasedIndex(uint32_t interval);

    uint8_t potId;
    uint32_t tsLastUpdate;
    uint16_t numValues;
    int8_t stepCount;
    bool encoderMode;

    static const uint16_t accelerationTable[9][10];
};
