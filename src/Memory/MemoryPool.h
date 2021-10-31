#pragma once

#include "MemoryBlock.h"
#include "helpers.h"
#include <cstring>

class MemoryPool
{
public:
    MemoryPool(Graphics *newFb, uint32_t address, size_t capacity);
    MemoryPool(uint32_t address, size_t capacity);
    ~MemoryPool();
    void assignStorageDriver(Graphics *newFb);
    MemoryBlock openMemoryBlock(void);
    uint32_t openAddress(void);

    uint32_t saveString(const char *text);
    size_t getString(uint32_t address, char *buffer, size_t maxLength);

    uint32_t saveItem(uint32_t bitmap, const char *text);
    size_t getItem(uint32_t address,
                   char *buffer,
                   size_t maxLength,
                   uint32_t *bitmap);

    void readData(uint32_t address, uint8_t *buffer, size_t maxLevel);

private:
    Graphics *fb;
    uint32_t getBaseAddress(void);
    bool write(const uint8_t *buffer, size_t length);
    void close(void);

    uint32_t baseAddress;
    size_t capacity;

    uint32_t newMemoryBlockAddress;
    uint32_t currentAddress;
    uint32_t currentLength;
    bool opened;

    friend class MemoryBlock;
};
