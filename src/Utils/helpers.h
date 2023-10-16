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
 * @file helpers.h
 *
 * @brief A collection of generic helper functions.
 */

#ifndef _HELPERS_H
#define _HELPERS_H

// \todo this really needs to be reworked and cleaned up!!!!!
// many CTRLV2 functions are present here
// JSON related functions belong to JSON parser domain
// colour handling belongs to Colours

#include <cstdlib>
#include <cstdint>
#include <vector>
#include "Rectangle.h"

// Name labels
#define COLOR_TILE_NAME_1 0xE77E
#define COLOR_TILE_NAME_2 0xF000
#define COLOR_TILE_NAME_3 0x72C2
#define COLOR_TILE_NAME_4 0xC663
#define COLOR_TILE_NAME_5 0x5FE0
#define COLOR_TILE_NAME_6 0x1B41
#define COLOR_TILE_NAME_7 0x29D3
#define COLOR_TILE_NAME_8 0x188D
#define COLOR_TILE_NAME_9 0x586B

#include <stdint.h>

enum ElementType { ELEMENT, ARRAY, OBJECT };

/*
 * logger enable flag
 */
extern volatile bool loggerEnabled;

#define helperGetPageId(id) ((id - 1) / 36 + 1)
#define helperGetSectionId(id) (((id - 1) % 36 / 12) + 1)
#define helperGetPotId(id) ((id - 1) % 12 + 1)

#define outOfRange(value, lowBound, highBound)                                 \
    !((value >= lowBound) && (value <= highBound))

const char *getNoteName(uint8_t noteNr);
int approxRollingAverage(int avg, int input);
void copyString(char *dest, const char *src, uint16_t maxLength);
void toUpperCase(char *str);
uint16_t translateColor(uint8_t colorId);
uint8_t translateColorToId(uint16_t colorRGB);
void byteToString(char *buff, uint8_t val);
void wordToString(char *buff, uint8_t val);
char nibbleToHex(uint8_t nibble);
void logData(uint8_t *data, uint16_t length, const char *prefix = "");
void logData(std::vector<uint8_t> data, const char *prefix = "");
void logSysex(std::vector<uint8_t> data, const char *prefix = "");
void logSysex(uint8_t *data,
              uint16_t length,
              const char *prefix = "",
              uint16_t offset = 0);
uint8_t getShift(uint8_t value);
uint16_t createMask(uint8_t pos, uint8_t size);
uint16_t getBackgroudColor(uint16_t fgColor);
void convertToHex(uint8_t data, char *outString);
void printHex(uint8_t *data, uint8_t length);
uint16_t getNextPowerOf2(uint16_t value);
uint16_t getRange(uint8_t bitWidth);
uint8_t reverse(uint8_t n);
Rectangle controlSlotToBounds(uint8_t slot);
uint8_t controlBoundsToSlot(Rectangle bounds);
uint8_t getControlSetIdFromScreenY(uint16_t y);

#endif /* _HELPERS_H */
