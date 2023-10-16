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
 * @file SysexBlock.cpp
 *
 * @brief A representation of a Sysex message of memory in E1 style
 *  memory pool.
 */

#include "SysexBlock.h"
#include "MemoryPool.h"

SysexBlock::SysexBlock()
{
}

SysexBlock::SysexBlock(MemoryPool *memoryPool, uint32_t address)
    : MemoryBlock(memoryPool, address)
{
}

SysexBlock::SysexBlock(const MemoryBlock &memoryBlock)
    : MemoryBlock(memoryBlock)
{
}

SysexBlock::~SysexBlock()
{
}

bool SysexBlock::isSysex(void) const
{
    return (header[0] == 0xF0);
}

bool SysexBlock::isEmpty(void) const
{
    return (length < 3);
}

uint32_t SysexBlock::getManufacturerSysexId(void) const
{
    uint32_t manufacturerSysexId = 0;

    if (length < 3) {
        return (0);
    }

    if (header[1] == 0) {
        manufacturerSysexId = (header[1] << 16) + (header[2] << 8) + header[3];
    } else {
        manufacturerSysexId = header[1];
    }

    return (manufacturerSysexId);
}

bool SysexBlock::isElectraSysex(void) const
{
    uint32_t manufacturerSysexId = getManufacturerSysexId();

    return (manufacturerSysexId == electraSroManufacturer);
}

ElectraCommand SysexBlock::getElectraSysexCommand(void) const
{
    ElectraCommand electraCommand;

    uint32_t manufacturerSysexId = getManufacturerSysexId();

    if (manufacturerSysexId == electraSroManufacturer) {
        electraCommand.set(
            header[4], header[5], header[6], header[7], header[8], header[9]);
    }

    return (electraCommand);
}

MemoryBlock SysexBlock::getElectraSysexPayload(void) const
{
    uint32_t offset = 0;
    uint32_t manufacturerSysexId = getManufacturerSysexId();

    if (manufacturerSysexId == electraSroManufacturer) {
        offset = 4;
    }

    offset += 2;

    return (getRange(offset, length - offset - 1));
}
