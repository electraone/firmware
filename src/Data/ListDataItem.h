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
 * @file ListDataItem.h
 *
 * @brief A entry of Selection Lists.
 */

#pragma once

#include <cstdint>
#include "Hardware.h"
#include "Bitmap.h"
#include "Base64.h"

class ListDataItem
{
public:
    ListDataItem()
        : value(0), address(0), labelIsEmpty(true), bitmapIsEmpty(true)
    {
    }

    ListDataItem(int16_t newValue,
                 const char *label,
                 const char *encodedBitmap = nullptr)
        : value(newValue), address(0), labelIsEmpty(false), bitmapIsEmpty(true)
    {
        Bitmap bitmap;

        if (encodedBitmap) {
            bitmap = saveBitmap(encodedBitmap);
            bitmapIsEmpty = false;
        }

        address =
            Hardware::memory.stringPool.saveItem(bitmap.getAddress(), label);
    }

    virtual ~ListDataItem() = default;

    int16_t getValue(void) const
    {
        return (value);
    }

    const char *getLabel(void) const
    {
        uint32_t bitmapAddress;

        Hardware::memory.stringPool.getItem(
            address, buffer, sizeof(buffer), &bitmapAddress);

        return (buffer);
    }

    const Bitmap getBitmap(void) const
    {
        if (bitmapIsEmpty) {
            return (Bitmap());
        }

        uint32_t bitmapAddress = 0;
        Hardware::memory.stringPool.getItem(
            address, buffer, sizeof(buffer), &bitmapAddress);

        return (Bitmap(bitmapAddress));
    }

    void paintBitmap(uint16_t x, uint16_t y, uint32_t colour)
    {
        Hardware::memory.bitmapPool.paint(getBitmap(), x, y, colour);
    }

    uint32_t getAddress(void) const
    {
        return (address);
    }

    bool isLabelEmpty(void) const
    {
        return (labelIsEmpty);
    }

    bool isBitmapEmpty(void) const
    {
        return (bitmapIsEmpty);
    }

private:
    /** Parse bitmap data
	 *
	 */
    Bitmap saveBitmap(const char *bitmap)
    {
        uint8_t bytes[255] = {};

        base64_decode((char *)bytes, (char *)bitmap, strlen(bitmap));

        return (Hardware::memory.bitmapPool.saveBitmap(bytes));
    }

    int16_t value;
    uint32_t address;

    struct {
        bool labelIsEmpty : 1;
        bool bitmapIsEmpty : 1;
    };

    static char buffer[40];
};
