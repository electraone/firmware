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
