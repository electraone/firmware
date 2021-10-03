#pragma once

#include <cstdint>

#define SDRAM_SIZE 16777216 // Reflects physical size of the SDRAM
#define RA8876_REG_AW_COLOR 0x5E // Color Depth of canvas & active window

class RA8876
{
public:
    RA8876(const uint8_t CSp, const uint8_t RSTp);
    ~RA8876();
    void setCanvasAddress(uint32_t address);
    void setAddress(uint32_t address);
    uint8_t readRAM8(void);
    void writeRAM8(uint8_t byte);
    uint16_t readRAM16(void);
    void writeRAM16(uint16_t value);
    void writeReg(uint8_t reg, uint8_t x);
    void fillRect(int x1, int y1, int width, int height, uint16_t color);
    uint32_t readRAM32(void);
    void writeRAM32(uint32_t value);

private:
    uint32_t currentAddress;
    uint8_t sdram[SDRAM_SIZE];
};
