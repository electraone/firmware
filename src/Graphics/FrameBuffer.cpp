#include "FrameBuffer.h"

FrameBuffer::FrameBuffer(const uint8_t CSp, const uint8_t RSTp)
    : RA8876(CSp, RSTp), writeLayer(1)
{
}

void FrameBuffer::switchWriteLayer(void)
{
    if (writeLayer == 1) {
        setCanvasAddress(FRAME_BUFFER_2);
        writeLayer = 2;
    } else {
        setCanvasAddress(FRAME_BUFFER_1);
        writeLayer = 1;
    }
}

void FrameBuffer::showPreparedLayer(int x, int y, int width, int height)
{
    setActiveWindowPosition(x, y);
    setActiveWindowSize(width, height);
    showPreparedLayer();
    syncLayers(x, y, width, height);
}

void FrameBuffer::showPreparedLayer(void)
{
    if (writeLayer == 1) {
        setMainWindowAddress(FRAME_BUFFER_1);
    } else {
        setMainWindowAddress(FRAME_BUFFER_2);
    }
}

void FrameBuffer::syncLayers(int x, int y, int width, int height)
{
    if (writeLayer == 1) {
        bteCopyNoWait(
            FRAME_BUFFER_1, x, y, FRAME_BUFFER_2, x, y, width, height);
    } else {
        bteCopyNoWait(
            FRAME_BUFFER_2, x, y, FRAME_BUFFER_1, x, y, width, height);
    }
}

uint32_t FrameBuffer::getActiveBufferAddress(void)
{
    return ((writeLayer == 1) ? FRAME_BUFFER_1 : FRAME_BUFFER_2);
}

void FrameBuffer::clear(void)
{
    switchWriteLayer();
    clearScreen(COLOR_BLACK);
    showPreparedLayer(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    switchWriteLayer();
}
