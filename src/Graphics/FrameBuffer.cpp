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
 * @file FrameBuffer.cpp
 *
 * @brief A videoRAM page repaint process management.
 */

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
