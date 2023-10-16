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
 * @file PotBroadcaster.h
 *
 * @brief A broadcaster of hardware potentiometer events.
 */

#pragma once

#include <map>
#include <cstdint>

class PotListener;
class PotEvent;
class Component;

typedef void (PotListener::*plFunction)(const PotEvent &);

class PotBroadcaster
{
public:
    PotBroadcaster();
    ~PotBroadcaster() = default;

    void addListener(PotListener *newListener);
    void removeListener(PotListener *listenerToRemove);
    void potChange(uint8_t potId, int16_t relativeChange);
    void potTouchDown(uint8_t potId);
    void potTouchUp(uint8_t potId);
    void stopPropagation(void);

private:
    void triggerCallbacks(PotEvent &pe, plFunction function);
    std::map<PotListener *, uint8_t> listeners;
    bool iteratorInvalidated;
};
