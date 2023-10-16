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
 * @file Mux.h
 *
 * @brief A hardware multiplexer management.
 */

#pragma once

#include "helpers.h"

// #define MUX_ENABLE  GPIOB_PDOR = GPIOB_PDOR | 0x10; delayMicroseconds(15);
// #define MUX_DISABLE GPIOB_PDOR = 0x00;

#define MUX_ENABLE                                                             \
    digitalWriteFast(49, LOW);                                                 \
    delayMicroseconds(1);
#define MUX_DISABLE digitalWriteFast(49, HIGH)

void configureMux(void);
void setMuxAddress(uint8_t address);
