#include "VideoRam.h"
#include "helpers.h"

VideoRam::VideoRam(const uint8_t CSp, const uint8_t RSTp)
    : FrameBuffer(CSp, RSTp)
{
}

void VideoRam::storeRam(uint32_t address, ByteSize size, uint32_t value)
{
#ifdef DEBUG
    logMessage("storeRam: address=%d, size=%d, value=%d", address, size, value);
#endif

    saveState();

    setMemoryMode(MemoryMode::LinearBpp8);
    setAddress(address);

    if (size == ByteSize::Uint8) {
        writeRAM8(value & 0xFF);
    } else if (size == ByteSize::Uint16) {
        writeRAM16(value & 0xFFFF);
    } else if (size == ByteSize::Uint32) {
        writeRAM32(value);
    }

    restoreState();
    setMemoryMode(DEFAULT_BPP);
}

void VideoRam::storeRam8(uint32_t address, uint8_t value)
{
    storeRam(address, ByteSize::Uint8, value);
}

void VideoRam::storeRam16(uint32_t address, uint16_t value)
{
    storeRam(address, ByteSize::Uint16, value);
}

void VideoRam::storeRam32(uint32_t address, uint32_t value)
{
    storeRam(address, ByteSize::Uint32, value);
}

uint32_t VideoRam::readRam(uint32_t address, ByteSize size)
{
    uint32_t value = 0;

    saveState();

    setMemoryMode(MemoryMode::LinearBpp8);
    setAddress(address);

    if (size == ByteSize::Uint8) {
        value = readRAM8() & 0xFF;
    } else if (size == ByteSize::Uint16) {
        value = readRAM16() & 0xFFFF;
    } else if (size == ByteSize::Uint32) {
        value = readRAM32();
    }

    restoreState();
    setMemoryMode(DEFAULT_BPP);

#ifdef DEBUG
    logMessage("readRam: address=%d, size=%d, value=%d", address, size, value);
#endif
    return (value);
}

uint8_t VideoRam::readRam8(uint32_t address)
{
    return (readRam(address, ByteSize::Uint8));
}

uint16_t VideoRam::readRam16(uint32_t address)
{
    return (readRam(address, ByteSize::Uint16));
}

uint32_t VideoRam::readRam32(uint32_t address)
{
    return (readRam(address, ByteSize::Uint32));
}

void VideoRam::readRamData(uint32_t address, uint8_t *data, uint16_t size)
{
#ifdef DEBUG
    logMessage("readRamData: address=%d", address);
#endif

    saveState();

    setMemoryMode(MemoryMode::LinearBpp8);
    setAddress(address);
    for (uint16_t i = 0; i < size; i++) {
        data[i] = readRAM8();
    }

    restoreState();
    setMemoryMode(DEFAULT_BPP);

#ifdef DEBUG
    logData(data, size, "data read:");
#endif
}

void VideoRam::writeRamData(uint32_t address,
                            const uint8_t *data,
                            uint16_t size)
{
#ifdef DEBUG
    logMessage("writeRamData: address=%d", address);
#endif

    saveState();

    setMemoryMode(MemoryMode::LinearBpp8);
    setAddress(address);
    for (uint16_t i = 0; i < size; i++) {
        writeRAM8(data[i]);
    }

    restoreState();
    setMemoryMode(DEFAULT_BPP);
}

void VideoRam::resetExtRam(void)
{
    noInterrupts();
    setForegroundColor(0);
    setCanvasAddress(SDRAM_PAGE_0);
    fillRect(0, 0, 1024, 600);
    setCanvasAddress(SDRAM_PAGE_1);
    fillRect(0, 0, 1024, 600);
    setCanvasAddress(SDRAM_PAGE_2);
    fillRect(0, 0, 1024, 600);
    setCanvasAddress(SDRAM_PAGE_3);
    fillRect(0, 0, 1024, 600);

#if DEFAULT_BPP < 24
    setCanvasAddress(SDRAM_PAGE_4);
    fillRect(0, 0, 1024, 600);
    setCanvasAddress(SDRAM_PAGE_5);
    fillRect(0, 0, 1024, 600);
    setCanvasAddress(SDRAM_PAGE_6);
    fillRect(0, 0, 1024, 600);
    setCanvasAddress(SDRAM_PAGE_7);
    fillRect(0, 0, 1024, 600);
    setCanvasAddress(SDRAM_PAGE_8);
    fillRect(0, 0, 1024, 600);
    setCanvasAddress(SDRAM_PAGE_9);
    fillRect(0, 0, 1024, 600);
#endif
    interrupts();
}
