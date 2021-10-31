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
