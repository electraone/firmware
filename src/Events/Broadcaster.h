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
