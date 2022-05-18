#pragma once

#include <map>
#include <cstdint>

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
        listeners[newListener] = 1;
    }

    void removeListener(T *listenerToRemove)
    {
        listeners.erase(listenerToRemove);
    }

protected:
    std::map<T *, uint8_t> listeners;
};
