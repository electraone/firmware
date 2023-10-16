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
 * @file Bitmap.h
 *
 * @brief A representation of bitmap data stored in the video RAM.
 */

#pragma once

#include "Rectangle.h"
#include "helpers.h"

class Bitmap : public Rectangle
{
public:
    Bitmap() : Rectangle(0, 0, 0, 0), bitmapIsEmpty(true)
    {
    }

    Bitmap(uint16_t newX, uint16_t newY, uint16_t newWidth, uint16_t newHeight)
        : Rectangle(newX, newY, newWidth, newHeight), bitmapIsEmpty(false)
    {
    }

    Bitmap(uint32_t address)
    {
        setAddress(address);
    }

    virtual ~Bitmap() = default;

    uint32_t getAddress(void) const
    {
        return ((getX() << 16) + getY());
    }

    void setAddress(uint32_t address)
    {
        bitmapIsEmpty = false;
        setPosition(address >> 16, address & 0xFFFF);
    }

    bool isEmpty(void) const
    {
        return (bitmapIsEmpty);
    }

private:
    bool bitmapIsEmpty;
};
