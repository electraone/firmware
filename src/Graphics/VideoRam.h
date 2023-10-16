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
 * @file VideoRam.h
 *
 * @brief A collection of functions to access the video RAM.
 */

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
