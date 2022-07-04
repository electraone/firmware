#pragma once

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

#define SYSEX_QUEUE_SIZE 5
extern CircularBuffer<SysexTransport, SYSEX_QUEUE_SIZE> sysexQueue;

void processSysex(void);
void processElectraSysex(uint8_t port, const SysexBlock &sysexBlock);
