#include "RA8876.h"
#include <cstring>

RA8876::RA8876(const uint8_t CSp, const uint8_t RSTp) : currentAddress(0)
{
    memset(sdram, 0x00, sizeof(sdram));
}

RA8876::~RA8876()
{
}

void RA8876::setCanvasAddress(uint32_t address)
{
}

void RA8876::setAddress(uint32_t address)
{
    currentAddress = address;
}

uint8_t RA8876::readRAM8(void)
{
    return (sdram[currentAddress++]);
}

void RA8876::writeRAM8(uint8_t byte)
{
    sdram[currentAddress++] = byte;
}

uint16_t RA8876::readRAM16(void)
{
    uint8_t valueLsb = sdram[currentAddress++];
    uint8_t valueMsb = sdram[currentAddress++];

    uint16_t value = (valueMsb << 8) + valueLsb;

    return (value);
}

void RA8876::writeRAM16(uint16_t value)
{
    sdram[currentAddress++] = value & 0xFF;
    sdram[currentAddress++] = (value >> 8);
}

void RA8876::writeReg(uint8_t reg, uint8_t x)
{
}

void RA8876::fillRect(int x1, int y1, int width, int height, uint16_t color)
{
}

uint32_t RA8876::readRAM32(void)
{
    return (0);
}

void RA8876::writeRAM32(uint32_t value)
{
}
