#pragma once

#include "FrameBuffer.h"
#include "TextBase.h"

class TextBTE : public virtual FrameBuffer
{
public:
    TextBTE(const uint8_t CSp, const uint8_t RSTp);
    virtual ~TextBTE() = default;

    void printText(uint16_t x,
                   uint16_t y,
                   const char *text,
                   TextStyle textStyle,
                   uint16_t width,
                   TextAlign align = TextAlign::left,
                   uint8_t color = 0);
    void textPlaceHolder(uint16_t x,
                         uint16_t y,
                         const char *text,
                         TextStyle style,
                         uint16_t width,
                         TextAlign align);
    static uint16_t getTextWidth(const char *text, TextStyle textStyle);

private:
    static uint16_t chromaColors[6];
    static uint16_t getCharSize(TextStyle textStyle);
    static uint16_t getCharPxHeight(TextStyle textStyle);
    static uint16_t getCharPositionY(TextStyle textStyle);
    static int8_t getCharacterTableIndex(char c);
    static uint8_t getCharacterWidth(char c, uint8_t size);
};
