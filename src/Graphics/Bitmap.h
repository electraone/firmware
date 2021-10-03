#pragma once

#include "Rectangle.h"
#include "helpers.h"

class Bitmap : public Rectangle
{
public:
    Bitmap() = default;

    Bitmap(uint16_t newX, uint16_t newY, uint16_t newWidth, uint16_t newHeight)
        : Rectangle(newX, newY, newWidth, newHeight)
    {
    }

    uint32_t getAddress(void)
    {
        return ((getX() << 16) + getY());
    }

    void setAddress(uint32_t address)
    {
        setPosition(address >> 16, address & 0xFFFF);
    }

/** \todo Temporary override to make it possible to compile ctrlv2 app
     */
#define NOT_SET_11 2047

    bool isEmpty(void)
    {
        return ((getX() == NOT_SET_11) ? true : false);
    }
};
