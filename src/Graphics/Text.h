#pragma once

#include "FrameBuffer.h"

#if DEFAULT_BPP == 24
#define FRAME_UI_TOOLKIT 0x004B0000
#else
#define FRAME_UI_TOOLKIT 0x00258000
#endif

enum class TextAlign { left, center, right };

enum class TextStyle {
    smallWhiteOnBlack = 0,
    mediumWhiteOnBlack = 13,
    mediumBlackOnWhite = 32,
    mediumWhiteOnDimmed = 52,
    largeWhiteOnBlack = 371,
    smallTransparent = 0,
    largeTransparent = 371,
    mediumTransparent = 13
};

class Text : public virtual FrameBuffer
{
public:
    Text(const uint8_t CSp, const uint8_t RSTp);
    virtual ~Text() = default;

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
