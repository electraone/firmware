#include "TaskProcessSysex.h"
#include "helpers.h"
#include "Sysex.h"
#include "App.h"

CircularBuffer<SysexTransport, SYSEX_QUEUE_SIZE> sysexQueue;

void processSysex(void)
{
    while (sysexQueue.isEmpty() != true) {
        SysexTransport st = sysexQueue.shift();

        if (st.transferType == SysexTransport::Type::File) {
            processSysexFile(st.port, st.file, st.type);
        } else if (st.transferType == SysexTransport::Type::Memory) {
            processSysexMemory(st.port, st.sysexBlock);
        }
    }
}

void processSysexFile(uint8_t port,
                      LocalFile &file,
                      ElectraCommand::Object type)
{
    bool status = true;

    App::get()->enableMidi = false;
    System::tasks.disableMidi();

    status = App::get()->handleCtrlFileReceived(port, file, type);

    System::tasks.enableMidi();
    App::get()->enableMidi = true;

    if (status == true) {
        sendAck(port);
    } else {
        sendNack(port);
    }
}

void processSysexMemory(uint8_t port, const SysexBlock &sysexBlock)
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

            if (object == ElectraCommand::Object::FilePreset) {
                sendPresetSlotChanged(port);
            }

            if (App::get()->handleCtrlFileReceived(port, file, object)
                == true) {
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
                if (sendSysExFile(
                        port, System::context.getCurrentPresetFile(), object)) {
                    logMessage("processElectraSysex::handleElectraSysex: "
                               "preset sysex sent to the host");
                }
            } else if (object == ElectraCommand::Object::FileLua) {
                if (sendSysExFile(
                        port, System::context.getCurrentLuaFile(), object)) {
                    logMessage("processElectraSysex::handleElectraSysex: "
                               "lua sysex sent to the host");
                }
            } else if (object == ElectraCommand::Object::FileConfig) {
                if (sendSysExFile(
                        port, System::context.getCurrentConfigFile(), object)) {
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
                sendElectraInfo(port,
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
                    sendPresetSlotChanged(port);
                }

                if (App::get()->handleCtrlFileRemoved(bankNumber, slot, object)
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
                logMessage("processElectraSysex: : switch to the update mode");
                System::tasks.displayUpdateModeScreen();
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
