#include "Sysex.h"
#include "Hardware.h"
#include "System.h"
#include "App.h"
#include "ArduinoJson.h"

// \todo stopFlush belongs to the System class
extern uint8_t stopFlush;

bool sendSysExFile(uint8_t port,
                   const char *filename,
                   ElectraCommand::Object fileType)
{
    FileIoStream file;
    uint32_t i = 0;
    uint32_t packet = 0;
    uint8_t byteInPacket = 0;
    uint8_t data[4];

    /*
	   * open file for download
	   */
    if (!(file = Hardware::sdcard.createInputStream(filename))) {
        logMessage("sendSysexFile: the file does not exists: filename=%s",
                   filename);
        return (false);
    }

    stopFlush = 1;
    sendSysExHeader(port);

    data[0] = FILE_UPLOAD;
    data[1] = (uint8_t)fileType;
    packet = 2;

    if (file != 0) {
        /*
		 * send content of the preset file
		 */
        while (file.available()) {
            byteInPacket = packet % 4;
            data[byteInPacket] = file.read();

            if (byteInPacket == 3) {
                sendSysExData(data, 4, port);
            }

            i++;
            packet++;

            if ((i % 20) == 0) {
                delayMicroseconds(50);
            }
        }

        file.close();
    }

    /*
	   * send last packet correctly
	   */
    byteInPacket = packet % 4;
    data[byteInPacket] = SYSEX_END;
    sendSysExData(data, byteInPacket + 1, port);

    stopFlush = 0;

    return (true);
}

void sendMidiLearn(uint8_t portToTransmitOn,
                   const char *msg,
                   uint8_t port,
                   uint8_t channel,
                   uint16_t parameterId,
                   uint16_t value)
{
    uint8_t data[512];
    StaticJsonDocument<512> doc;

    logMessage(
        "sendMidiLearn: msg=%s, port=%d, channel=%d, parameter=%d, value=%d",
        msg,
        port,
        channel,
        parameterId,
        value);

    stopFlush = 1;
    sendSysExHeader(portToTransmitOn);
    data[0] = MIDILEARN_SWITCH;

    char *jsonStart = (char *)&data[1];

    doc["msg"] = msg;
    doc["port"] = port;
    doc["channel"] = channel;
    doc["parameterId"] = parameterId;
    doc["value"] = value;

    serializeJson(doc, jsonStart, 512);

    size_t jsonSize = strlen(jsonStart);
    data[jsonSize + 1] = SYSEX_END;
    sendSysExData(data, jsonSize + 2, portToTransmitOn);

    stopFlush = 0;
}

void sendMidiLearnSysex(uint8_t portToTransmitOn,
                        uint8_t port,
                        const SysexBlock &sysexBlock)
{
    const size_t capacity = JSON_ARRAY_SIZE(1024) + 64;
    size_t sysExLength = sysexBlock.getLength();
    uint8_t data[sysExLength * 5 + 64];
    char byteBuffer[3];
    StaticJsonDocument<capacity> doc;

    stopFlush = 1;
    sendSysExHeader(portToTransmitOn);
    data[0] = MIDILEARN_SWITCH;

    char *jsonStart = (char *)&data[1];

    doc["msg"] = "sysex";
    doc["port"] = port;

    JsonArray array = doc["data"].to<JsonArray>();

    for (uint16_t i = 0; i < sysExLength; i++) {
        convertToHex(sysexBlock.peek(i), byteBuffer);

        if (!array.add(byteBuffer)) {
            logMessage("failed: %d", i);
        }
    }

    serializeJson(doc, jsonStart, sysExLength * 5 + 65);

    size_t jsonSize = strlen(jsonStart);
    data[jsonSize + 1] = SYSEX_END;
    sendSysExData(data, jsonSize + 2, portToTransmitOn);

    stopFlush = 0;
}

void sendElectraInfo(uint8_t port,
                     const char *electraInfoSerial,
                     uint8_t electraInfoHwRevision)
{
    uint8_t data[128];
    char buf[16];
    StaticJsonDocument<128> doc;

    sprintf(buf,
            "%d.%02d",
            electraInfoHwRevision / 100,
            electraInfoHwRevision - 200);

    stopFlush = 1;
    sendSysExHeader(port);

    data[0] = FILE_UPLOAD;
    data[1] = ELECTRA_INFO;

    char *jsonStart = (char *)&data[2];

    doc["versionText"] = FIRMWARE_VERSION;
    doc["versionSeq"] = FIRMWARE_SEQ;
    doc["serial"] = electraInfoSerial;
    doc["hwRevision"] = buf;

    serializeJson(doc, jsonStart, 120);

    size_t jsonSize = strlen(jsonStart);

    data[jsonSize + 2] = SYSEX_END;
    sendSysExData(data, jsonSize + 3, port);

    stopFlush = 0;
}

void sendMemoryInfo(uint8_t port)
{
    uint8_t data[128];
    StaticJsonDocument<128> doc;

    stopFlush = 1;
    sendSysExHeader(port);

    data[0] = FILE_UPLOAD;
    data[1] = MEMORY_INFO;

    char *jsonStart = (char *)&data[2];

    doc["freePercentage"] = Hardware::ram.getFreeRam();

    serializeJson(doc, jsonStart, 120);

    size_t jsonSize = strlen(jsonStart);

    data[jsonSize + 2] = SYSEX_END;
    sendSysExData(data, jsonSize + 3, port);

    stopFlush = 0;
}

void sendAppInfo(uint8_t port)
{
    uint8_t data[256];
    StaticJsonDocument<256> doc;

    stopFlush = 1;
    sendSysExHeader(port);

    data[0] = FILE_UPLOAD;
    data[1] = APP_INFO;

    char *jsonStart = (char *)&data[2];

    doc["app"] = App::get()->getApplicationName();

    if (System::context.getPresetName()) {
        doc["preset"] = System::context.getPresetName();
    }

    serializeJson(doc, jsonStart, 250);

    size_t jsonSize = strlen(jsonStart);

    data[jsonSize + 2] = SYSEX_END;
    sendSysExData(data, jsonSize + 3, port);

    stopFlush = 0;
}

void sendNack(uint8_t port)
{
    uint8_t data[5];

    stopFlush = 1;
    sendSysExHeader(port);

    data[0] = EVENT_MESSAGE;
    data[1] = NACK;
    data[2] = 0x00;
    data[3] = 0x00;
    data[4] = SYSEX_END;

    sendSysExData(data, 5, port);

    stopFlush = 0;
}

void sendAck(uint8_t port)
{
    uint8_t data[5];

    stopFlush = 1;
    sendSysExHeader(port);

    data[0] = EVENT_MESSAGE;
    data[1] = ACK;
    data[2] = 0x00;
    data[3] = 0x00;
    data[4] = SYSEX_END;
    sendSysExData(data, 5, port);

    stopFlush = 0;
}

void sendPresetSwitched(uint8_t port, uint8_t bankNumber, uint8_t slotId)
{
    uint8_t data[5];

    stopFlush = 1;

    sendSysExHeader(port);
    data[0] = EVENT_MESSAGE;
    data[1] = PRESET_SWITCH;
    data[2] = bankNumber;
    data[3] = slotId;
    data[4] = SYSEX_END;
    sendSysExData(data, 5, port);

    stopFlush = 0;
}

void sendSnapshotChanged(uint8_t port)
{
    uint8_t data[3];

    stopFlush = 1;

    sendSysExHeader(port);
    data[0] = EVENT_MESSAGE;
    data[1] = SNAPSHOT_CHANGE;
    data[2] = SYSEX_END;
    sendSysExData(data, 3, port);

    stopFlush = 0;
}

void sendSnapshotBankChanged(uint8_t port, uint8_t bankNumber)
{
    uint8_t data[4];

    stopFlush = 1;

    sendSysExHeader(port);
    data[0] = EVENT_MESSAGE;
    data[1] = SNAPSHOT_BANK_SWITCH;
    data[2] = bankNumber;
    data[3] = SYSEX_END;
    sendSysExData(data, 4, port);

    stopFlush = 0;
}

void sendPresetSlotChanged(uint8_t port)
{
    uint8_t data[3];

    stopFlush = 1;

    sendSysExHeader(port);
    data[0] = EVENT_MESSAGE;
    data[1] = PRESET_LIST_CHANGE;
    data[2] = SYSEX_END;
    sendSysExData(data, 3, port);

    stopFlush = 0;
}

/*
 * send USB MIDI packets
 */
void sendSysExData(const uint8_t *data, uint32_t length, uint8_t port)
{
    uint8_t cable = (port & 0x0F) << 4;

    while (length > 3) {
        usb_midi_write_packed(0x04 | cable | (data[0] << 8) | (data[1] << 16)
                              | (data[2] << 24));
        data += 3;
        length -= 3;
    }
    if (length == 3) {
        usb_midi_write_packed(0x07 | cable | (data[0] << 8) | (data[1] << 16)
                              | (data[2] << 24));
    } else if (length == 2) {
        usb_midi_write_packed(0x06 | cable | (data[0] << 8) | (data[1] << 16));
    } else if (length == 1) {
        usb_midi_write_packed(0x05 | cable | (data[0] << 8));
    }
}

void sendSysExHeader(uint8_t port)
{
    uint8_t data[4];
    uint32_t manufacturerSysexId = SYSEX_MANUFACTURER;

    data[0] = SYSEX_START;
    data[1] = (manufacturerSysexId >> 16) & 0xFF;
    data[2] = (manufacturerSysexId >> 8) & 0xFF;
    data[3] = manufacturerSysexId & 0xFF;

    sendSysExData(data, 4, port);
}
