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

    ListDataItem(uint16_t newValue,
                 const char *label,
                 const char *encodedBitmap)
        : address(0), labelIsEmpty(false), bitmapIsEmpty(true)
    {
        Bitmap bitmap;

        if (encodedBitmap) {
            bitmap = saveBitmap(encodedBitmap);
            bitmapIsEmpty = false;
        }

        address =
            Hardware::memory.stringPool.saveItem(bitmap.getAddress(), label);
        value = newValue;
    }

    virtual ~ListDataItem() = default;

    uint16_t getValue(void) const
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

    uint16_t value;
    uint32_t address;

    struct {
        bool labelIsEmpty : 1;
        bool bitmapIsEmpty : 1;
    };

    static char buffer[40];
};
