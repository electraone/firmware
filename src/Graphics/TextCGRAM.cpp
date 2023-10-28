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
 * @file TextCGRAM.cpp
 *
 * @brief A CGRAM based texts.
 */

#include "TextCGRAM.h"
#include "Colours.h"
#include "Fonts/Font9ptBold.h"

TextCGRAM::TextCGRAM(const uint8_t CSp, const uint8_t RSTp)
    : FrameBuffer(CSp, RSTp)
{
}

void TextCGRAM::loadCGRAMFonts(void)
{
    setCGRAMAddress(SDRAM_CGRAM);

    saveState();
    setMemoryMode(RA8876::MemoryMode::LinearBpp8);
    setAddress(SDRAM_CGRAM);

    for (uint16_t i = 0; i < sizeof(font9ptBold); i++) {
        writeRAM8(font9ptBold[i]);
    }

    restoreState();
    setMemoryMode(RA8876::DEFAULT_BPP);
}

int TextCGRAM::getStringWidth(const char *text)
{
    size_t numCharacters = strlen(text);
    int stringWidth = 0;
    uint8_t characterWidth = 0;

    for (size_t i = 0; i < numCharacters; i++) {
        characterWidth = font9ptBoldCharWidth[text[i] - 32];
        stringWidth += characterWidth;
    }

    // Add extra padding for the last character
    // This is required to make sure the text will fit in the
    // bounding box.
    uint8_t extraPadding = (16 - characterWidth);

    return (stringWidth + extraPadding);
}

void TextCGRAM::print(uint16_t x,
                      uint16_t y,
                      const char *text,
                      uint16_t width,
                      TextAlign align)
{
    uint16_t xPosition = 0;
    uint16_t textWidth = getStringWidth(text);

    if (textWidth > width) {
        textWidth = width;
    }

    //drawRect(x, y, width, 20);

    if (align == TextAlign::center) {
        xPosition = (width - textWidth) / 2;
    } else if (align == TextAlign::right) {
        xPosition = x + width - textWidth;
    } else {
        xPosition = x;
    }

    uint16_t buffer[64];
    size_t textLength = strlen(text);

    for (size_t i = 0; i < textLength; i++) {
        buffer[i] = text[i] - 32; // offset the ascii code
    }

    printProportional(xPosition, y, buffer, textLength);
}

void TextCGRAM::printFixed(uint16_t x,
                           uint16_t y,
                           const uint16_t *text,
                           size_t size)
{
    setCursor(x, y);
    setTextMode();
    write(text, size);
    setGraphicsMode();
}

void TextCGRAM::printProportional(uint16_t x,
                                  uint16_t y,
                                  const uint16_t *text,
                                  size_t size)
{
    for (size_t i = 0; i < size; i++) {
        setCursor(x, y);
        setTextMode();
        writeChar(text[i]);
        setGraphicsMode();
        x += font9ptBoldCharWidth[text[i]];
    }
}
