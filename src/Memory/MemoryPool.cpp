#include "MemoryPool.h"

MemoryPool::MemoryPool(Graphics *newFb, uint32_t address, size_t capacity)
    : fb(newFb),
      baseAddress(address),
      capacity(capacity),
      newMemoryBlockAddress(0),
      currentAddress(0),
      currentLength(0),
      opened(false)
{
}

MemoryPool::MemoryPool(uint32_t address, size_t capacity)
    : fb(nullptr),
      baseAddress(address),
      capacity(capacity),
      newMemoryBlockAddress(0),
      currentAddress(0),
      currentLength(0),
      opened(false)
{
}

MemoryPool::~MemoryPool()
{
}

void MemoryPool::assignStorageDriver(Graphics *newFb)
{
    fb = newFb;
}

MemoryBlock MemoryPool::openMemoryBlock(void)
{
    if (opened == false) {
        opened = true;
    }

    return (MemoryBlock(this, currentAddress));
}

uint32_t MemoryPool::openAddress(void)
{
    if (opened == false) {
        opened = true;
    }

    return (currentAddress);
}

uint32_t MemoryPool::getBaseAddress(void)
{
    return (baseAddress);
}

bool MemoryPool::write(const uint8_t *buffer, size_t length)
{
    if (opened == false) {
        return (false);
    }

    fb->writeRamData(baseAddress + currentAddress, buffer, length);

    currentAddress += length;
    currentLength += length;

    return (true);
}

uint32_t MemoryPool::saveString(const char *text)
{
    noInterrupts();
    uint32_t address = openAddress();

    write((uint8_t *)text, strlen(text) + 1);
    close();
    interrupts();

    return (address);
}

uint32_t MemoryPool::saveItem(uint32_t bitmap, const char *text)
{
    noInterrupts();
    uint32_t address = openAddress();

    write((uint8_t *)&bitmap, 4);
    write((uint8_t *)text, strlen(text) + 1);
    close();
    interrupts();

    return (address);
}

size_t MemoryPool::getString(uint32_t address, char *buffer, size_t maxLength)
{
    fb->readRamData(baseAddress + address, (uint8_t *)buffer, maxLength);
    buffer[maxLength - 1] = '\0';

    return (strlen(buffer));
}

size_t MemoryPool::getItem(uint32_t address,
                           char *buffer,
                           size_t maxLength,
                           uint32_t *bitmap)
{
    fb->readRamData(baseAddress + address, (uint8_t *)bitmap, 4);
    fb->readRamData(baseAddress + address + 4, (uint8_t *)buffer, maxLength);
    buffer[maxLength - 1] = '\0';

    return (strlen(buffer));
}

void MemoryPool::close(void)
{
    if (newMemoryBlockAddress > capacity) {
        currentAddress = 0;
    }

    newMemoryBlockAddress = currentAddress;
    currentLength = 0;
    opened = false;
}
