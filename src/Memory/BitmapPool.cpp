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

    // restore the color depth to 16
    fb->writeReg(RA8876_REG_AW_COLOR, 1);

    currentX += (BITMAP_WIDTH / 8);

    if (currentX > (1024 - (BITMAP_WIDTH / 8))) {
        currentX = 0;
        currentY += BITMAP_HEIGHT;
    }

    return (Bitmap(bitmapX, bitmapY, BITMAP_WIDTH, BITMAP_HEIGHT));
}

void BitmapPool::paint(Bitmap &bitmap,
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

void BitmapPool::paint(Bitmap &bitmap, uint16_t x, uint16_t y, uint16_t colour)
{
    uint32_t address = fb->getActiveBufferAddress();
    paint(bitmap, x, y, colour, address);
}
