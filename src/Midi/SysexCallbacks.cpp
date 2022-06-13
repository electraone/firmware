#include "SysexCallbacks.h"
#include "SysexHandler.h"
#include "SysexHandlerCtrl.h"

static SysexHandler handlerMidiIo1(MidiInterface::Type::MidiIo, 0);
static SysexHandler handlerMidiIo2(MidiInterface::Type::MidiIo, 1);
static SysexHandler handlerMidiUsbDev1(MidiInterface::Type::MidiUsbDev, 0);
static SysexHandler handlerMidiUsbDev2(MidiInterface::Type::MidiUsbDev, 1);
static SysexHandler handlerMidiUsbHost1(MidiInterface::Type::MidiUsbHost, 0);
static SysexHandler handlerMidiUsbHost2(MidiInterface::Type::MidiUsbHost, 1);
static SysexHandlerCtrl handlerMidiUsbDevCtrl(MidiInterface::Type::MidiUsbDev,
                                              2);

void handleMidiIoSysExPort0(const uint8_t *sysExData,
                            uint16_t sysExSize,
                            bool complete)
{
    handlerMidiIo1.process(sysExData, sysExSize, complete);
}

void handleMidiIoSysExPort1(const uint8_t *sysExData,
                            uint16_t sysExSize,
                            bool complete)
{
    handlerMidiIo2.process(sysExData, sysExSize, complete);
}

void handleMidiUsbDevSysEx(uint8_t cable,
                           const uint8_t *sysExData,
                           uint16_t sysExSize,
                           bool complete)
{
    // \todo acceoting Electra SysEx messages on non-CTRL port will be replaced
    // with message routing. This is a quick fix for now, to allow @jhh working
    // on his ableton preset.
    static bool electraCommand = false;
    static uint8_t electraCommandPort = 0;

    if (sysExData[0] == 0xF0 && sysExData[1] == 0x00 && sysExData[2] == 0x21
        && sysExData[3] == 0x45) {
        electraCommand = true;
        electraCommandPort = cable;
    }

    if (electraCommand && cable == electraCommandPort) {
        handlerMidiUsbDevCtrl.process(sysExData, sysExSize, complete);
    } else {
        if (cable == 0) {
            handlerMidiUsbDev1.process(sysExData, sysExSize, complete);
        } else if (cable == 1) {
            handlerMidiUsbDev2.process(sysExData, sysExSize, complete);
        } else if (cable == 2) {
            handlerMidiUsbDevCtrl.process(sysExData, sysExSize, complete);
        }
    }

    if (complete && cable == electraCommandPort) {
        electraCommand = false;
    }
}

void handleMidiUsbHostSysExPort0(const uint8_t *sysExData,
                                 uint16_t sysExSize,
                                 bool complete)
{
    handlerMidiUsbHost1.process(sysExData, sysExSize, complete);
}

void handleMidiUsbHostSysExPort1(const uint8_t *sysExData,
                                 uint16_t sysExSize,
                                 bool complete)
{
    handlerMidiUsbHost2.process(sysExData, sysExSize, complete);
}
