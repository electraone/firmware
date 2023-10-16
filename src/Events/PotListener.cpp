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
 * @file PotListener.cpp
 *
 * @brief A listener of hardware potentiometer events.
 */

#include "PotListener.h"
#include "helpers.h"

PotBroadcaster PotListener::potBroadcaster;

PotListener::PotListener()
    : potId(AllPots),
      tsLastUpdate(0),
      numValues(0),
      stepCount(0),
      encoderMode(0)
{
}

void PotListener::stopPropagation(void)
{
    potBroadcaster.stopPropagation();
}

void PotListener::assignPot(uint8_t newPotId, uint16_t newNumValues)
{
    potId = newPotId;
    numValues = newNumValues;
    potBroadcaster.addListener(this);
}

void PotListener::assignAllPots(uint16_t newNumValues)
{
    potId = AllPots;
    numValues = newNumValues;
    potBroadcaster.addListener(this);
}

void PotListener::enableEncoderMode(bool newEncoderMode)
{
    encoderMode = newEncoderMode;
}

void PotListener::releasePot(void)
{
    potBroadcaster.removeListener(this);
}

uint8_t PotListener::getPotId(void)
{
    return (potId);
}

void PotListener::setNumValues(uint16_t newNumValues)
{
    numValues = newNumValues;
}

uint16_t PotListener::getNumValues(void)
{
    return (numValues);
}

void PotListener::resetStepCount(void)
{
    stepCount = 0;
}

int16_t PotListener::computeRate(int16_t relativeChange)
{
    if (numValues < 63
        || encoderMode) { // this is for the controls with low number of values
        return (decreaseRate(relativeChange));
    } else {
        uint32_t ts = millis();
        uint32_t tp = ts - tsLastUpdate;
        tsLastUpdate = ts;

        uint8_t tableId = getAccelerationTable(numValues);
        uint8_t index = getIntervalBasedIndex(tp);
        return (accelerationTable[tableId][index] * relativeChange);
    }
}

int16_t PotListener::decreaseRate(int16_t relativeChange)
{
    int16_t acceleratedChange = 0;
    stepCount += relativeChange;

    uint16_t threshold = 0;

    if (encoderMode) {
        threshold =
            constrain((1 << std::max(0, __builtin_clz(numValues) - 24)), 1, 8)
            * 2;
    } else {
        threshold =
            constrain((1 << std::max(0, __builtin_clz(numValues) - 27)), 1, 16);
    }
    /*
    System::logger.write(LOG_ERROR, "threshold: %d, stepCount: %d: _b: %d",
               threshold,
               stepCount,
               __builtin_clz(numValues));*/
    if (abs(stepCount) >= threshold) {
        acceleratedChange = ((stepCount > 0) ? 1 : -1);
        stepCount = 0;
        return (acceleratedChange);
    }

    return (0);
}

uint8_t PotListener::getAccelerationTable(uint16_t numValues)
{
    // clz on ARM uses 32bits
    return (std::max(0, 26 - __builtin_clz(numValues & 0x3FFF)));
}

uint8_t PotListener::getIntervalBasedIndex(uint32_t interval)
{
    return ((100 - std::min((uint32_t)100, (interval / 10) * 10)) / 10);
}

const uint16_t PotListener::accelerationTable[9][10] = {
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 2 },
    { 1, 1, 1, 1, 1, 1, 2, 2, 2, 3 },
    { 1, 1, 1, 2, 2, 3, 4, 6, 8, 10 },
    { 1, 2, 3, 4, 5, 6, 7, 9, 11, 14 },
    { 1, 2, 3, 4, 6, 8, 10, 13, 18, 25 },
    { 1, 2, 4, 8, 10, 13, 20, 32, 48, 64 },
    { 1, 2, 4, 8, 16, 24, 32, 48, 64, 128 },
    { 1, 4, 8, 16, 32, 48, 64, 96, 128, 256 },
    { 1, 8, 16, 32, 64, 96, 128, 192, 256, 512 }
};
