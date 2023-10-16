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
 * @file BitmapTools.cpp
 *
 * @brief A collections of functions to manipulate videoRAM bitmaps.
 */

#include "BitmapTools.h"
#include "Colours.h"

BitmapTools::BitmapTools(const uint8_t CSp, const uint8_t RSTp)
    : FrameBuffer(CSp, RSTp)
{
}

BitmapTools::~BitmapTools()
{
}

void BitmapTools::copy(uint32_t srcAddress,
                       uint16_t srcX,
                       uint16_t srcY,
                       uint32_t destAddress,
                       uint16_t destX,
                       uint16_t destY,
                       uint16_t width,
                       uint16_t height)
{
    bteCopyChroma(
        srcAddress, srcX, srcY, destAddress, destX, destY, width, height);
}

void BitmapTools::copyFromToolkit(uint16_t srcX,
                                  uint16_t srcY,
                                  uint16_t destX,
                                  uint16_t destY,
                                  uint16_t width,
                                  uint16_t height)
{
    uint32_t address = getActiveBufferAddress();

    bteCopyChroma(
        FRAME_UI_TOOLKIT, srcX, srcY, address, destX, destY, width, height);
}

void BitmapTools::dim(uint16_t x,
                      uint16_t y,
                      uint16_t width,
                      uint16_t height,
                      uint32_t tintColour)
{
    uint32_t address = getCanvasAddress();

    dim(address, x, y, width, height, tintColour);
}

void BitmapTools::dim(uint32_t address,
                      uint16_t x,
                      uint16_t y,
                      uint16_t width,
                      uint16_t height,
                      uint32_t tintColour)
{
    setCanvasAddress(FRAME_MASK);
    setForegroundColor(tintColour);
    fillRect(0, 0, width, height);
    bteCopyRop(FRAME_MASK,
               activeWindowX,
               activeWindowY,
               address,
               x,
               y,
               address,
               x,
               y,
               width,
               height,
               0xAA);
    setCanvasAddress(address);
}

void BitmapTools::dimAll(void)
{
    uint32_t address = getCanvasAddress();

    dim(address,
        0,
        0,
        FrameBuffer::getActiveWindowWidth(),
        FrameBuffer::getActiveWindowHeight(),
        Colours565::black);
}

void BitmapTools::backdrop(uint16_t x,
                           uint16_t y,
                           uint16_t width,
                           uint16_t height,
                           uint32_t backgroundColour)
{
    uint32_t address = getCanvasAddress();

    setCanvasAddress(FRAME_MASK);
    setForegroundColor(backgroundColour);
    fillRect(0, 0, width, height);
    bteCopyRop(FRAME_MASK,
               activeWindowX,
               activeWindowY,
               address,
               x,
               y,
               address,
               x,
               y,
               width,
               height,
               0xE2);
    setCanvasAddress(address);
}
