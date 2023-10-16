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

#pragma once

#define CIRCULAR_BUFFER_INT_SAFE
#include "CircularBuffer.h"
#include "LocalFile.h"
#include "SysexBlock.h"
#include "ElectraCommand.h"

struct SysexTransport {
    enum Type { File, Memory, None };

    SysexTransport() : transferType(None)
    {
    }

    SysexTransport(LocalFile newFile,
                   ElectraCommand::Object newType,
                   uint8_t newPort)
        : transferType(File), file(newFile), type(newType), port(newPort)
    {
    }

    SysexTransport(SysexBlock newSysexBlock, uint8_t newPort)
        : transferType(Memory), sysexBlock(newSysexBlock), port(newPort)
    {
    }

    ~SysexTransport() = default;

    Type transferType;
    LocalFile file;
    SysexBlock sysexBlock;
    ElectraCommand::Object type;
    uint8_t port;
};

#define SYSEX_QUEUE_SIZE 32
extern CircularBuffer<SysexTransport, SYSEX_QUEUE_SIZE> sysexQueue;

void processSysex(void);
void processSysexFile(uint8_t port,
                      LocalFile &file,
                      ElectraCommand::Object type);
void processSysexMemory(uint8_t port, const SysexBlock &sysexBlock);
