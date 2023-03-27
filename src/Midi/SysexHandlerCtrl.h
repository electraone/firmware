#pragma once

#include "SysexHandler.h"
#include "LocalFile.h"
#include "System.h"
#include "Base64.h"

enum class FileType { preset, config, luaScript, uiToolkit, unknown };

class SysexHandlerCtrl : private SysexHandler
{
public:
    SysexHandlerCtrl(MidiInterface::Type newMidiInterface, uint8_t newPort)
        : SysexHandler(newMidiInterface, newPort),
          fileType(ElectraCommand::Object::Unknown),
          writeToFile(false),
          filename(nullptr)
    {
    }

    virtual ~SysexHandlerCtrl() = default;

    void process(const uint8_t *sysExData,
                 uint16_t sysExSize,
                 bool complete,
                 int8_t portOverride = -1) override
    {
        uint16_t decodedDataSize = 0;

        if (packetNr == 0) {
            App::get()->enableMidi = false;

            // if it is a upload of a file
            if ((sysExData[4] == (uint8_t)ElectraCommand::Type::FileUpload)
                && ((sysExData[5]
                     == (uint8_t)ElectraCommand::Object::FilePresetLegacy)
                    || (sysExData[5]
                        == (uint8_t)ElectraCommand::Object::FilePreset)
                    || (sysExData[5]
                        == (uint8_t)ElectraCommand::Object::FileLua)
                    || (sysExData[5]
                        == (uint8_t)ElectraCommand::Object::FileUi))) {
                System::tasks.enableCtrlMidi();

                // write preset directly to the current preset file
                if (sysExData[5] == (uint8_t)ElectraCommand::Object::FileUi) {
                    System::logger.write(
                        LOG_ERROR, "handleSysEx: toolkit transfer initiated");
                    filename = UI_FILE;
                    fileType = ElectraCommand::Object::FileUi;
                } else if (sysExData[5]
                           == (uint8_t)ElectraCommand::Object::FileLua) {
                    filename = System::context.getCurrentLuaFile();
                    fileType = ElectraCommand::Object::FileLua;
                    System::logger.write(
                        LOG_ERROR,
                        "handleSysEx: lua script transfer initiated: filename=%s",
                        filename);
                } else if ((sysExData[5]
                            == (uint8_t)ElectraCommand::Object::FilePreset)
                           || (sysExData[5]
                               == (uint8_t)
                                   ElectraCommand::Object::FilePresetLegacy)) {
                    filename = System::context.getCurrentPresetFile();
                    fileType = ElectraCommand::Object::FilePreset;
                    System::logger.write(
                        LOG_ERROR,
                        "handleSysEx: preset transfer initiated: filename=%s",
                        filename);
                } else {
                    System::logger.write(LOG_ERROR,
                                         "handleSysEx: unknown filetype");
                    fileType = ElectraCommand::Object::Unknown;
                }

                /*
    				     * Open the file
    				     */
                if (filename) {
                    file = Hardware::sdcard.createOutputStream(
                        filename, FILE_WRITE | O_CREAT | O_TRUNC);
                }

                // if file cannot be opened now, use sysexBlock
                if (file) {
                    writeToFile = true;

                    // strip the leading bytes
                    sysExData += 6; // remove sysex header
                    sysExSize -= 6;

                    if (fileType == ElectraCommand::Object::FileUi) {
                        sysExData +=
                            2; // the ui toolkit header has two extra bytes
                        sysExSize -= 2;
                    }
                } else {
                    System::logger.write(LOG_ERROR,
                                         "handleSysEx: Read file failed: %s",
                                         filename);
                    writeToFile = false;
                }
            }

            /*
    		 * always create a sysex block
    		 */
            sysexBlock = SysexBlock(App::get()->sysexPool.openMemoryBlock());
        }

        /*
    	 * Process each packet according to if it is a preset transfer or other sysex message
    	 */
        if (writeToFile) {
            // Check if it is a last packet with 0xF7. If so, trim it
            if (sysExData[sysExSize - 1] == 0xF7) {
                sysExSize -= 1; // remove sysex stop byte
            }

            // if it is a uitoolkitTransfer decode the base64 data
            if (fileType == ElectraCommand::Object::FileUi) {
                decodedDataSize = base64_decode(
                    (char *)decodedData, (char *)sysExData, sysExSize);
                file.write(decodedData, decodedDataSize);
            } else {
                file.write(sysExData, sysExSize);
            }
        } else {
            sysexBlock.writeBytes(sysExData, sysExSize);
        }
        packetNr++;

        /*
    	 * handle last packet of the sysex transfer
    	 */
        if (complete) {
            //bool callbackStatus = true;
            uint8_t portUsed = (portOverride != -1) ? portOverride : port;

            // Close the sysex block
            sysexBlock.close();

            System::tasks.disableMidi();

            if (writeToFile) {
                file.close();
                sysexQueue.push(
                    SysexTransport(LocalFile(filename), fileType, portUsed));
            } else {
                sysexQueue.push(SysexTransport(sysexBlock, portUsed));
            }

            App::get()->enableMidi = true;
            System::tasks.enableMidi();

            if (fileType == ElectraCommand::Object::FilePreset) {
                // Loading the preset always (temporary fix) removes the Lua script
                // ie. the Lua script needs to be transferred afterwards
                Hardware::sdcard.deleteFile(
                    System::context.getCurrentLuaFile());
            }

            packetNr = 0;
            fileType = ElectraCommand::Object::Unknown;
            writeToFile = false;
            filename = nullptr;
        }
    }

private:
    File file;
    ElectraCommand::Object fileType;
    bool writeToFile;
    uint8_t decodedData[USB_MIDI_SYSEX_MAX];
    const char *filename;
};
