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
 * @file Broadcaster.h
 *
 * @brief A base template class for Broadcasters.
 */

#pragma once

#include <map>
#include <cstdint>
#include "helpers.h"

template <class T>
class Broadcaster
{
public:
    Broadcaster()
    {
    }
    virtual ~Broadcaster() = default;

    void addListener(T *newListener)
    {
        listeners[newListener] = 1; // make active immedia
    }

    void removeListener(T *listenerToRemove)
    {
        listeners.erase(listenerToRemove);
    }

    void suspendListener(T *listenerToSuspend)
    {
        listeners[listenerToSuspend] = 0; // suspend
    }

    void resumeListener(T *listenerToSuspend)
    {
        listeners[listenerToSuspend] = 1; // make active
    }

protected:
    std::map<T *, uint8_t> listeners;
};
