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
 * @file MemoryBlock.cpp
 *
 * @brief A representation of a chunk of memory in E1 style memory pool.
 */

#include "MemoryBlock.h"
#include "MemoryPool.h"
#include <cstring>

MemoryBlock::MemoryBlock()
    : memoryPool(nullptr),
      startAddress(0),
      currentAddress(0),
      endAddress(0),
      length(0),
      headerLength(0),
      isFullMessageInHeader(false)
{
}

MemoryBlock::MemoryBlock(MemoryPool *newMemoryPool, uint32_t newAddress)
    : memoryPool(newMemoryPool),
      startAddress(newAddress),
      currentAddress(0),
      endAddress(0),
      length(0),
      headerLength(0),
      isFullMessageInHeader(false)
{
}

MemoryBlock::~MemoryBlock()
{
}

void MemoryBlock::seek(uint32_t address)
{
    currentAddress = startAddress + address;
}

uint32_t MemoryBlock::tell(void) const
{
    return (currentAddress - startAddress);
}

int MemoryBlock::read()
{
    int byte = -1;

    if (memoryPool == nullptr) {
        return (-1);
    }

    if (currentAddress < endAddress) {
        size_t bytePosition = tell(); //currentAddress - startAddress;

        if (bytePosition < headerLength) {
            byte = header[bytePosition];
        } else {
            byte = memoryPool->fb->readRam8(memoryPool->getBaseAddress()
                                            + currentAddress);
        }

        currentAddress++;
    } else {
        byte = -1;
    }

    return (byte);
}

size_t MemoryBlock::readBytes(uint8_t *buffer, size_t length)
{
    size_t totalReadBytes = 0;
    size_t readBytes = 0;
    size_t offset = 0;

    while ((readBytes = readBytesInternal(buffer + offset, length - offset))
           != 0) {
        offset += readBytes;
        totalReadBytes += readBytes;
    }
    return (totalReadBytes);
}

size_t MemoryBlock::writeBytes(const uint8_t *buffer, size_t lengthToWrite)
{
    if (memoryPool == nullptr) {
        return (0);
    }

    size_t bytesAvailableInHeader = headerMaxSize - headerLength;
    size_t bytesExceedingHeader;
    size_t bytesToWriteToHeader;
    size_t originalHeaderLength = headerLength;

    if (bytesAvailableInHeader > lengthToWrite) {
        bytesExceedingHeader = 0;
        bytesToWriteToHeader = lengthToWrite;
    } else {
        bytesExceedingHeader = lengthToWrite - bytesAvailableInHeader;
        bytesToWriteToHeader = bytesAvailableInHeader;
    }

    if (bytesToWriteToHeader > 0) {
        memcpy(header + headerLength, buffer, bytesToWriteToHeader);
        headerLength += bytesToWriteToHeader;
        bytesAvailableInHeader = headerMaxSize - headerLength;

        if ((isFullMessageInHeader == false) && (bytesAvailableInHeader == 0)) {
            isFullMessageInHeader = true;
            memoryPool->write(header, headerLength);

            if (bytesExceedingHeader > 0) {
                memoryPool->write(buffer
                                      + (headerLength - originalHeaderLength),
                                  bytesExceedingHeader);
            }
        }
    } else {
        memoryPool->write(buffer, lengthToWrite);
    }

    currentAddress += lengthToWrite;
    length += lengthToWrite;
    endAddress = startAddress + length;

    return (lengthToWrite);
}

void MemoryBlock::close(void)
{
    seek(0);

    if (this->memoryPool) {
        this->memoryPool->close();
    }
}

size_t MemoryBlock::getLength(void) const
{
    return (length);
}

MemoryBlock MemoryBlock::getRange(uint32_t rangeStart,
                                  uint32_t rangeLength) const
{
    MemoryBlock newBlock;

    newBlock.memoryPool = memoryPool;
    newBlock.startAddress = startAddress + rangeStart;
    newBlock.currentAddress = newBlock.startAddress;
    newBlock.length = rangeLength;
    newBlock.endAddress = newBlock.startAddress + newBlock.length;

    if (rangeStart < headerMaxSize) {
        size_t lengthInHeader = headerMaxSize - rangeStart;

        memcpy(newBlock.header, header + rangeStart, lengthInHeader);
        memoryPool->fb->readRamData(memoryPool->getBaseAddress() + startAddress
                                        + rangeStart + lengthInHeader,
                                    newBlock.header + lengthInHeader,
                                    headerMaxSize - lengthInHeader);
    } else {
        memoryPool->fb->readRamData(memoryPool->getBaseAddress() + startAddress
                                        + rangeStart,
                                    newBlock.header,
                                    headerMaxSize);
    }

    newBlock.headerLength =
        (rangeLength > headerMaxSize) ? headerMaxSize : rangeLength;

    return (newBlock);
}

bool MemoryBlock::isEmpty(void) const
{
    return (length == 0 ? true : false);
}

bool MemoryBlock::isAllocatedInPool(void) const
{
    return (memoryPool != nullptr ? true : false);
}

void MemoryBlock::print(MemoryBlockOutputType outputType)
{
    if (memoryPool == nullptr) {
        return;
    }

    uint8_t buffer[200];
    uint16_t bytesRead = 0;

    seek(0);

    // logMessage ("Extram header: startAddress=%d, length=%d", startAddress, length);
    while ((bytesRead = readBytes(buffer, sizeof(buffer))) > 0) {
        // logMessage ("Bytes read %d", bytesRead);
        if (outputType == MemoryBlockOutputType::Chars) {
            logChars((uint8_t *)buffer, bytesRead);
        } else {
            logData((uint8_t *)buffer, bytesRead);
        }
    }

    if (outputType == MemoryBlockOutputType::Chars) {
        Serial.print("\n");
    }
    seek(0);
}

int MemoryBlock::peek(size_t position) const
{
    uint8_t byte = -1;

    if (memoryPool == nullptr) {
        return (-1);
    }

    size_t addressToRead = startAddress + position;

    if (position < headerLength) {
        byte = header[position];
    } else {
        byte = memoryPool->fb->readRam8(memoryPool->getBaseAddress()
                                        + addressToRead);
    }

    return (byte);
}

size_t MemoryBlock::readBytesInternal(uint8_t *buffer, size_t length)
{
    if (memoryPool == nullptr) {
        return (0);
    }

    uint16_t bytesToRead = 0;

    size_t currentPosition = tell();

    if (currentPosition < headerLength) {
        bytesToRead = (headerLength - currentPosition) > length
                          ? length
                          : headerLength - currentPosition;

        if (bytesToRead) {
            memcpy((void *)buffer, header + currentPosition, bytesToRead);
            currentAddress += bytesToRead;
        }
    } else {
        bytesToRead = (endAddress - currentAddress) >= length
                          ? length
                          : (endAddress - currentAddress);

        if (bytesToRead) {
            memoryPool->fb->readRamData(memoryPool->getBaseAddress()
                                            + currentAddress,
                                        buffer,
                                        bytesToRead);
            currentAddress += bytesToRead;
        }
    }

    return (bytesToRead);
}
