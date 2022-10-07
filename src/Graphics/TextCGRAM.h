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
