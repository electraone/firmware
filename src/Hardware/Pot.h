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
 * @file Pot.h
 *
 * @brief A representation of a hardware Potentiometer.
 */

#pragma once

#include <ADC.h>

extern ADC *adc;

/*
 * Pot class
 *
 * a representation of the physical turning knob
 */
class Pot
{
public:
    Pot(uint8_t id);
    void process(void);
    void initialise(void);

    uint8_t id;
    int16_t step;
    bool active;
    bool touched;

private:
    int16_t A;
    int16_t B;
    int16_t pA;
    int16_t pB;
    uint8_t address;

    static constexpr uint8_t sensitivity = 16;
};
