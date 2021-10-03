#pragma once

#include "settings.h"
#include "Pot.h"
#include "PotTouch.h"

class Pots
{
public:
    Pots();
    ~Pots() = default;

    Pot &operator[](const uint8_t id);
    Pot *getNextActivePot(void);
    Pot *getNextPot(void);
    void rescanAllPots(void);
    void initialise(void);

private:
    Pot pots[NR_OF_HW_POTS] = {
        Pot(0), Pot(1), Pot(2), Pot(3), Pot(4),  Pot(5),
        Pot(6), Pot(7), Pot(8), Pot(9), Pot(10), Pot(11)
    };
};
