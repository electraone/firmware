#pragma once

#include "FrameBuffer.h"

typedef uint32_t address_t;

class VideoRam : public virtual FrameBuffer
{
public:
    enum ByteSize { Uint8, Uint16, Uint32 };

    VideoRam(const uint8_t CSp, const uint8_t RSTp);
    ~VideoRam() = default;

    void storeRam8(uint32_t address, uint8_t value);
    void storeRam16(uint32_t address, uint16_t value);
    void storeRam32(uint32_t address, uint32_t value);
    uint8_t readRam8(uint32_t address);
    uint16_t readRam16(uint32_t address);
    uint32_t readRam32(uint32_t address);

    void readRamData(uint32_t address, uint8_t *data, uint16_t size);
    void writeRamData(uint32_t address, const uint8_t *data, uint16_t size);
    void resetExtRam(void);

private:
    void storeRam(uint32_t address, ByteSize size, uint32_t value);
    uint32_t readRam(uint32_t address, ByteSize size);
};
