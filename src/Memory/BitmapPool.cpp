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
 * @file BitmapPool.cpp
 *
 * @brief A representation of the E1 style memory pool to hold Bitmap data.
 */

#include "BitmapPool.h"
#include "Colours.h"
#include "Graphics.h"

BitmapPool::BitmapPool(FrameBuffer *newFb, uint32_t address, size_t capacity)
    : fb(newFb), baseAddress(address), currentX(0), currentY(0)
{
}

BitmapPool::BitmapPool(uint32_t address, size_t capacity)
    : fb(nullptr), baseAddress(address), currentX(0), currentY(0)
{
}

void BitmapPool::assignStorageDriver(FrameBuffer *newFb)
{
    fb = newFb;
}

void BitmapPool::clear(void)
{
    currentX = 0;
    currentY = 0;
}

// \todo this belongs to the RA8876 lib
Bitmap BitmapPool::saveBitmap(const uint8_t *data)
{
    uint16_t bitmapX = currentX;
    uint16_t bitmapY = currentY;

    fb->setCanvasAddress(baseAddress);

    // set the colour depth to 8
    fb->writeReg(RA8876_REG_AW_COLOR, 0);

    for (uint8_t y = 0; y < BITMAP_HEIGHT; y++) {
        fb->setAddress(bitmapX, bitmapY + y);

        for (uint8_t x = 0; x < 6; x++) {
            fb->writeRAM8(reverse(data[x + (y * 6)]));
        }
    }

    // \todo this is not ok, it should return the default colour bit depth
    // restore the color depth to 16
    fb->writeReg(RA8876_REG_AW_COLOR, 1);

    currentX += (BITMAP_WIDTH / 8);

    if (currentX > (1024 - (BITMAP_WIDTH / 8))) {
        currentX = 0;
        currentY += BITMAP_HEIGHT;
    }

    return (Bitmap(bitmapX, bitmapY, BITMAP_WIDTH, BITMAP_HEIGHT));
}

void BitmapPool::paint(const Bitmap &bitmap,
                       uint16_t x,
                       uint16_t y,
                       uint16_t colour,
                       uint32_t address)
{
    fb->bteExpansion(baseAddress,
                     bitmap.getX(),
                     bitmap.getY(),
                     address,
                     x,
                     y,
                     BITMAP_WIDTH,
                     BITMAP_HEIGHT,
                     colour);
}

void BitmapPool::paint(const Bitmap &bitmap,
                       uint16_t x,
                       uint16_t y,
                       uint16_t colour)
{
    uint32_t address = fb->getActiveBufferAddress();
    paint(bitmap, x, y, colour, address);
}
