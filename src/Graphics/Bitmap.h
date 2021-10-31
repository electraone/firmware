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
