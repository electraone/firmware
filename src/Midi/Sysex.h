#pragma once

#include "ElectraCommand.h"
#include "SysexBlock.h"

#define SYSEX_START 0xF0
#define SYSEX_END 0xF7
#define SYSEX_MANUFACTURER 0x002145

/*
 * Commands
 */
#define FILE_UPLOAD 0x01
#define FILE_DOWNLOAD 0x02
#define MIDILEARN_SWITCH 0x03
#define UPDATE 0x04
#define REMOVE 0x05
#define SWAP 0x06
#define CREATE 0x07
#define EXECUTE 0x08
#define UPDATE_RUNTIME 0x14
#define DATA_CONTAINER 0x7D
#define EVENT_MESSAGE 0x7E
#define SYSTEM_CALL 0x7F

/*
 * Events
 */
#define NACK 0x00
#define ACK 0x01
#define PRESET_SWITCH 0x02
#define SNAPSHOT_CHANGE 0x03
#define SNAPSHOT_BANK_SWITCH 0x04
#define PRESET_LIST_CHANGE 0x05

/*
 * Object types
 */
#define FILE_PRESET_LEGACY 0x00
#define FILE_PRESET 0x01
#define FILE_CONFIG 0x02
#define FILE_SNAPSHOT 0x03
#define PRESET_LIST 0x04
#define SNAPSHOT_LIST 0x05
#define SNAPSHOT_INFO 0x06
#define CONTROL 0x07
#define FILE_LUA_SCRIPT 0x0C
#define FUNCTION 0x0D
#define FILE_UITOOLKIT 0x70
#define APP_INFO 0x7C
#define LOGGER 0x7D
#define MEMORY_INFO 0x7E
#define ELECTRA_INFO 0x7F

#define USB_MIDI_PORT_CTRL 2

bool sendSysExFile(uint8_t port,
                   const char *filename,
                   ElectraCommand::Object fileType);
void sendMidiLearn(uint8_t portToTransmitOn,
                   const char *msg,
                   uint8_t port,
                   uint8_t channel,
                   uint16_t parameterId,
                   uint16_t value);
void sendMidiLearnSysex(uint8_t portToTransmitOn,
                        uint8_t port,
                        const SysexBlock &sysexBlock);
void sendElectraInfo(uint8_t port,
                     const char *electraInfoSerial,
                     uint8_t electraInfoHwRevision);
void sendMemoryInfo(uint8_t port);
void sendAppInfo(uint8_t port);
void sendNack(uint8_t port);
void sendAck(uint8_t port);
void sendPresetSwitch(uint8_t port, uint8_t bankNumber, uint8_t slotId);
void sendSnapshotChange(uint8_t port);
void sendSnapshotBankChange(uint8_t port, uint8_t bankNumber);
void sendPresetSlotChange(uint8_t port);

void sendSysExData(const uint8_t *data, uint32_t length, uint8_t port);
void sendSysExHeader(uint8_t port);
