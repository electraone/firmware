#pragma once

#include "helpers.h"
#include "MemoryBlock.h"
#include "ElectraCommand.h"

class SysexBlock : public MemoryBlock
{
public:
    SysexBlock();
    SysexBlock(MemoryPool *memoryPool, uint32_t address);
    SysexBlock(const MemoryBlock &memoryBlock);
    ~SysexBlock();

    bool isSysex(void) const;
    uint32_t getManufacturerSysexId(void) const;
    bool isElectraSysex(void) const;
    ElectraCommand getElectraSysexCommand(void) const;
    MemoryBlock getElectraSysexPayload(void) const;

private:
    // \todo this needs to be deduplicated
    static const uint32_t nonProfitManufacturer = 0x7d0000;
    static const uint32_t electraSroManufacturer = 0x002145;
};
