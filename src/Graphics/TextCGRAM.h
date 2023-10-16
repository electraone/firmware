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
 * @file TextCGRAM.h
 *
 * @brief A CGRAM based texts.
 */

#pragma once

#include "FrameBuffer.h"
#include "TextBase.h"

class TextCGRAM : public virtual FrameBuffer
{
public:
    TextCGRAM(const uint8_t CSp, const uint8_t RSTp);
    virtual ~TextCGRAM() = default;

    void loadCGRAMFonts(void);
    int getStringWidth(const char *text);
    void print(uint16_t x,
               uint16_t y,
               const char *text,
               uint16_t width,
               TextAlign align = TextAlign::left);

private:
    void printFixed(uint16_t x, uint16_t y, const uint16_t *text, size_t size);
    void printProportional(uint16_t x,
                           uint16_t y,
                           const uint16_t *text,
                           size_t size);
};
