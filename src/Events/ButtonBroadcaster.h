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
 * @file ButtonBroadcaster.h
 *
 * @brief A broadcaster of hardware button events.
 */

#pragma once

#include "Broadcaster.h"

class ButtonListener;

typedef void (ButtonListener::*blFunction)(uint8_t);

class ButtonBroadcaster : public Broadcaster<ButtonListener>
{
public:
    //using Broadcaster<ButtonListener>::Broadcaster;

    void buttonDown(uint8_t buttonId);
    void buttonUp(uint8_t buttonId);
    void buttonLongHold(uint8_t buttonId);

    void listListeners(void);

private:
    void triggerCallbacks(uint8_t buttonId, blFunction function);
};
