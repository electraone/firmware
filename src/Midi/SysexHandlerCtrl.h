#pragma once

#include "SysexHandler.h"
#include "Sysex.h"
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
                    logMessage("handleSysEx: toolkit transfer initiated");
                    filename = UI_FILE;
                    fileType = ElectraCommand::Object::FileUi;
                } else if (sysExData[5]
                           == (uint8_t)ElectraCommand::Object::FileLua) {
                    filename = System::context.getCurrentLuaFile();
                    fileType = ElectraCommand::Object::FileLua;
                    logMessage(
                        "handleSysEx: lua script transfer initiated: filename=%s",
                        filename);
                } else if ((sysExData[5]
                            == (uint8_t)ElectraCommand::Object::FilePreset)
                           || (sysExData[5]
                               == (uint8_t)
                                   ElectraCommand::Object::FilePresetLegacy)) {
                    filename = System::context.getCurrentPresetFile();
                    fileType = ElectraCommand::Object::FilePreset;
                    logMessage(
                        "handleSysEx: preset transfer initiated: filename=%s",
                        filename);
                } else {
                    logMessage("handleSysEx: unknown filetype");
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
                    logMessage("handleSysEx: Read file failed: %s", filename);
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
            bool callbackStatus = true;
            uint8_t portUsed = (portOverride != -1) ? portOverride : port;

            // Close the sysex block
            sysexBlock.close();

            System::tasks.disableMidi();

            if (writeToFile) {
                file.close();

                LocalFile receivedFile(filename);

                callbackStatus =
                    App::get()->handleCtrlFileReceived(receivedFile, fileType);

                App::get()->enableMidi = true;
                System::tasks.enableMidi();

                if (callbackStatus == true) {
                    sendAck(portUsed);
                } else {
                    sendNack(portUsed);
                }
            } else {
                processElectraSysex(portUsed, sysexBlock);
                App::get()->enableMidi = true;
                System::tasks.enableMidi();
            }

            if (fileType == ElectraCommand::Object::FilePreset
                || fileType == ElectraCommand::Object::FileLua) {
                sendPresetSlotChange(portUsed);
            }

            packetNr = 0;
            fileType = ElectraCommand::Object::Unknown;
            writeToFile = false;
            filename = nullptr;
        }
    }

    static void processElectraSysex(uint8_t port, const SysexBlock &sysexBlock)
    {
        if (sysexBlock.isElectraSysex()) {
            ElectraCommand cmd = sysexBlock.getElectraSysexCommand();
            ElectraCommand::Object object = cmd.getObject();
            MemoryBlock sysexPayload = sysexBlock.getElectraSysexPayload();

            if (cmd.isFileUpload()) {
                LocalFile file;

                if (object == ElectraCommand::Object::FilePreset) {
                    file.setFilepath(System::context.getCurrentPresetFile());
                } else if (object == ElectraCommand::Object::FileConfig) {
                    file.setFilepath(System::context.getCurrentTempFile());
                } else if (object == ElectraCommand::Object::FileLua) {
                    file.setFilepath(System::context.getCurrentLuaFile());
                } else {
                    file.setFilepath(System::context.getCurrentTempFile());
                }

                file.writeAll(sysexPayload);

                if (object == ElectraCommand::Object::FilePreset
                    || object == ElectraCommand::Object::FileLua) {
                    sendPresetSlotChange(port);
                }

                if (App::get()->handleCtrlFileReceived(file, object) == true) {
                    logMessage(
                        "processElectraSysex::handleElectraSysex: sending ACK");
                    sendAck(port);
                } else {
                    logMessage(
                        "processElectraSysex::handleElectraSysex: sending NACK");
                    sendNack(port);
                }
            }
            if (cmd.isFileRequest()) {
                if (object == ElectraCommand::Object::FilePreset) {
                    if (sendSysExFile(port,
                                      System::context.getCurrentPresetFile(),
                                      object)) {
                        logMessage("processElectraSysex::handleElectraSysex: "
                                   "preset sysex sent to the host");
                    }
                } else if (object == ElectraCommand::Object::FileLua) {
                    if (sendSysExFile(port,
                                      System::context.getCurrentLuaFile(),
                                      object)) {
                        logMessage("processElectraSysex::handleElectraSysex: "
                                   "lua sysex sent to the host");
                    }
                } else if (object == ElectraCommand::Object::FileConfig) {
                    if (sendSysExFile(port,
                                      System::context.getCurrentConfigFile(),
                                      object)) {
                        logMessage("processElectraSysex::handleElectraSysex: "
                                   "config sysex sent to the host");
                    }
                } else if (object == ElectraCommand::Object::MemoryInfo) {
                    sendMemoryInfo(port);
                    logMessage("processElectraSysex::handleElectraSysex: "
                               "memoryInfo sysex sent to the host");
                } else if (object == ElectraCommand::Object::AppInfo) {
                    sendAppInfo(port);
                    logMessage("processElectraSysex::handleElectraSysex: "
                               "appInfo sysex sent to the host");
                } else if (object == ElectraCommand::Object::ElectraInfo) {
                    sendElectraInfo(
                        port,
                        System::runtimeInfo.getElectraInfoSerial(),
                        System::runtimeInfo.getElectraInfoHwRevision());
                    logMessage("processElectraSysex::handleElectraSysex: "
                               "electraInfo sysex sent to the host");
                } else {
                    App::get()->handleElectraSysex(
                        port,
                        sysexBlock); // this is to call application based requests
                }
            } else if (cmd.isExecute()) {
                if (L) {
                    logMessage("processElectraSysex::handleElectraSysex: "
                               "execute function");
                    uint8_t buffer[sysexPayload.getLength()];
                    sysexPayload.readBytes(buffer, sysexPayload.getLength());

                    buffer[sysexPayload.getLength()] = '\0';

                    logMessage("processElectraSysex::handleElectraSysex: "
                               "execute Lua: \"%s\"",
                               buffer);
                    runLuaString((char *)buffer);
                    sendAck(port);
                } else {
                    logMessage("processElectraSysex::handleElectraSysex: "
                               "Lua script is not available");
                }
            } else if (cmd.isRemove()) {
                uint8_t bankNumber = cmd.getByte1();
                uint8_t slot = cmd.getByte2();
                uint8_t fileNumber = bankNumber * 12 + slot;
                char filename[MAX_FILENAME_LENGTH];
                char *fileToRemove = nullptr;

                if (object == ElectraCommand::Object::FilePreset) {
                    System::context.formatPresetFilename(
                        filename, MAX_FILENAME_LENGTH, fileNumber);
                    fileToRemove = filename;
                } else if (object == ElectraCommand::Object::FileLua) {
                    System::context.formatLuaFilename(
                        filename, MAX_FILENAME_LENGTH, fileNumber);
                    fileToRemove = filename;
                } else {
                    logMessage("processElectraSysex: "
                               "file type not handled by the base firmware");
                    fileToRemove = nullptr;
                }

                if (fileToRemove) {
                    if (Hardware::sdcard.exists(fileToRemove)) {
                        int status = Hardware::sdcard.deleteFile(fileToRemove);
                        logMessage("processElectraSysex: removing file %s: %s",
                                   fileToRemove,
                                   (status == true) ? "OK" : "fail");
                        sendPresetSlotChange(port);
                    }

                    if (App::get()->handleCtrlFileRemoved(
                            bankNumber, slot, object)
                        == true) {
                        logMessage(
                            "processElectraSysex::handleElectraSysex: sending ACK");
                        sendAck(port);
                    } else {
                        logMessage(
                            "processElectraSysex::handleElectraSysex: sending NACK");
                        sendNack(port);
                    }
                } else {
                    logMessage(
                        "processElectraSysex: removed command passed to the app");
                    App::get()->handleElectraSysex(port, sysexBlock);
                }
            } else if (cmd.isSystemCall()) {
                if ((uint8_t)object == 0x7F) {
                    logMessage(
                        "processElectraSysex: : switch to the update mode");
                    delay(200);
                    _reboot_Teensyduino_();
                } else if (object == ElectraCommand::Object::Logger) {
                    if (cmd.getByte1() == 0) {
                        logMessage("processElectraSysex::LOGGER OFF");
                        loggerEnabled = false;
                    } else {
                        loggerEnabled = true;
                        logMessage("processElectraSysex::LOGGER ON");
                    }
                    System::runtimeInfo.setLoggerStatus(loggerEnabled);
                    sendAck(port);
                }
            } else {
                App::get()->handleElectraSysex(port, sysexBlock);
            }
        } else {
            logMessage("processElectraSysex: "
                       "message ignored. Not an Electra.One message");
        }
    }

private:
    File file;
    ElectraCommand::Object fileType;
    bool writeToFile;
    uint8_t decodedData[USB_MIDI_SYSEX_MAX];
    const char *filename;
};
