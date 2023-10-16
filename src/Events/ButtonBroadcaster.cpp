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
 * @file ButtonBroadcaster.cpp
 *
 * @brief A broadcaster of hardware button events.
 */

#include "ButtonBroadcaster.h"
#include "ButtonListener.h"
#include "System.h"

void ButtonBroadcaster::buttonDown(uint8_t buttonId)
{
    triggerCallbacks(buttonId, &ButtonListener::onButtonDown);
}

void ButtonBroadcaster::buttonUp(uint8_t buttonId)
{
    triggerCallbacks(buttonId, &ButtonListener::onButtonUp);
}

void ButtonBroadcaster::buttonLongHold(uint8_t buttonId)
{
    triggerCallbacks(buttonId, &ButtonListener::onButtonLongHold);
}

void ButtonBroadcaster::triggerCallbacks(uint8_t buttonId, blFunction function)
{
    auto i = listeners.begin();

    while (i != listeners.end()) {
        auto current = i++;
        auto listener = current->first;

        if (listeners[listener] == 1) {
            if ((listener->buttonId == ButtonListener::AllButtons)
                || (buttonId == listener->buttonId)) {
                (listener->*function)(buttonId);
            }
        }
    }
}

void ButtonBroadcaster::listListeners(void)
{
    for (auto i = listeners.begin(); i != listeners.end(); i++) {
        auto listener = i->first;
        System::logger.write(LOG_ERROR,
                             "listListeners: buttonListener: %x is active: %d",
                             listener,
                             listeners[listener]);
    }
}
