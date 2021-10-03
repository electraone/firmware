#pragma once

#include "ElectraCommand.h"

#define SYSEX_START 0xF0
#define SYSEX_END 0xF7
#define SYSEX_MANUFACTURER 0x002145

bool sendSysExFile(const char *filename, ElectraCommand::Object fileType);
void sendElectraInfo(const char *electraInfoSerial,
                     uint8_t electraInfoHwRevision);
void sendMemoryInfo(void);
void sendAppInfo(void);
void sendNack(void);
void sendAck(void);
void sendSysExData(const uint8_t *data, uint32_t length, uint8_t cable);

void sendSysExHeader(void);

// Kept here temporarily for compatibility reasons

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
