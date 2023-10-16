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
#include "Pot.h"
#include "Mux.h"

/** @todo: extract platform specific code to a ADC driver. */

Pot::Pot(uint8_t newId)
    : id(newId),
      step(0),
      active(false),
      touched(false),
      A(0),
      B(0),
      pA(0),
      pB(0),
      address(newId)
{
}

void Pot::process(void)
{
    setMuxAddress(this->address);
    MUX_ENABLE;
    A = Hardware::adc.adc0->analogRead(A0);
    B = Hardware::adc.adc1->analogRead(A16);
    MUX_DISABLE;

    //System::logger.write(LOG_ERROR, "A=%d B=%d", A, B);

    uint16_t move = max(abs(B - pB), abs(A - pA));

    if (move > sensitivity) {
        move /= sensitivity;

        if (A <= 1) {
            step = ((B - pB) > 0) ? -move : move;
        } else if (A == 1023) {
            step = ((B - pB) > 0) ? move : -move;
        } else if (B < 512) {
            step = ((A - pA) > 0) ? move : -move;
        } else if (B > 512) {
            step = ((A - pA) > 0) ? -move : move;
        } else {
            step = 0;
        }

        pA = A;
        pB = B;
    } else {
        step = 0;
    }
}

void Pot::initialise(void)
{
    setMuxAddress(this->address);
    MUX_ENABLE;
    A = Hardware::adc.adc0->analogRead(A0);
    B = Hardware::adc.adc1->analogRead(A16);
    MUX_DISABLE;
    pA = this->A;
    pB = this->B;
    active = false;
}
