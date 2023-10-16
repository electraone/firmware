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

#include "Hardware.h"
#include "System.h"

Pots::Pots()
{
}

Pot &Pots::operator[](const uint8_t id)
{
    return (pots[id]);
}

Pot *Pots::getNextActivePot(void)
{
    static uint8_t currentPotId = 0;
    uint8_t indexToScan;

    Pot *pot = NULL;

    for (uint8_t i = 0; i < NR_OF_HW_POTS; i++) {
        indexToScan = (currentPotId + i) % NR_OF_HW_POTS;
        if (pots[indexToScan].active == true) {
            pot = &pots[indexToScan];
            break;
        }
    }
    currentPotId = indexToScan + 1;

    if (currentPotId > (MAX_POT_ID)) {
        currentPotId = 0;
    }

    return (pot);
}

Pot *Pots::getNextPot(void)
{
    static uint8_t currentPotId = 0;

    Pot *pot = &pots[currentPotId];

    currentPotId++;

    if (currentPotId >= (MAX_POT_ID)) {
        currentPotId = 0;
    }

    return (pot);
}

void Pots::rescanAllPots(void)
{
    for (uint8_t i = 0; i < NR_OF_HW_POTS; i++) {
        pots[i].initialise();
    }
}

void Pots::initialise(void)
{
    // Initialise Pot Touch
    Hardware::potTouch.begin();
    Hardware::potTouch.reset();

    // Configure Pot Touch
    if (!Hardware::potTouch.init()) {
        System::logger.write(
            LOG_ERROR,
            "capacitive sensing: cannot calibrate. Check input USB voltage");
    } else {
        System::logger.write(LOG_ERROR, "capacitive sensing: Initialized");
    }

    // Run the first scan of Pots to get them to known state
    rescanAllPots();
}
