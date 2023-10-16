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
 * @file BitmapPool.h
 *
 * @brief A representation of the E1 style memory pool to hold Bitmap data.
 */

#pragma once

#include "helpers.h"
#include "Bitmap.h"
#include "FrameBuffer.h"

#define BITMAP_WIDTH 48
#define BITMAP_HEIGHT 18

class BitmapPool
{
public:
    BitmapPool(FrameBuffer *newFb, uint32_t address, size_t capacity);
    BitmapPool(uint32_t address, size_t capacity);

    void assignStorageDriver(FrameBuffer *newFb);
    Bitmap saveBitmap(const uint8_t *data);
    void paint(const Bitmap &bitmap, uint16_t x, uint16_t y, uint16_t colour);
    void paint(const Bitmap &bitmap,
               uint16_t x,
               uint16_t y,
               uint16_t colour,
               uint32_t address);
    void clear(void);

private:
    FrameBuffer *fb;
    uint32_t baseAddress;
    uint16_t currentX;
    uint16_t currentY;
};
