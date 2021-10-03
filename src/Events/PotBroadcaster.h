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

private:
    void triggerCallbacks(PotEvent &pe, plFunction function);
    std::map<PotListener *, uint8_t> listeners;
};
