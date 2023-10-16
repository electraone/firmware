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
 * @file Mux.cpp
 *
 * @brief A hardware multiplexer management.
 */

#include "Mux.h"
#include "core_pins.h"

void configureMux(void)
{
    pinMode(16, OUTPUT);
    pinMode(17, OUTPUT);
    pinMode(18, OUTPUT);
    pinMode(19, OUTPUT);
    pinMode(49, OUTPUT);
    pinMode(41, INPUT); // button_scan
}

void setMuxAddress(uint8_t address)
{
    // GPIOB_PDOR = potId;

    digitalWriteFast(18, address & 0b1000);
    digitalWriteFast(19, address & 0b0100);
    digitalWriteFast(17, address & 0b0010);
    digitalWriteFast(16, address & 0b0001);
}
