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
    if (cable == 0) {
        handlerMidiUsbDev1.process(sysExData, sysExSize, complete);
    } else if (cable == 1) {
        handlerMidiUsbDev2.process(sysExData, sysExSize, complete);
    } else if (cable == 2) {
        handlerMidiUsbDevCtrl.process(sysExData, sysExSize, complete);
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
