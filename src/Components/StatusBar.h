#pragma once

#include "Component.h"
#include "MidiInterface.h"
#include "MidiMessage.h"

enum class Direction { in, out };

extern USBDevice USBDevices[];

class StatusBar : public Component
{
public:
    StatusBar();
    ~StatusBar();

    void paint(Graphics &g);
    void forceRepaint(void);
    void indicate(MidiInterface::Type interface,
                  uint8_t port,
                  Direction direction,
                  MidiMessage::Type msgType);
    void indicateUsbHostChange(void);
    bool repaintIsNeeded(void);

private:
    void statusBarItem(Graphics &g,
                       uint16_t x,
                       uint16_t y,
                       const char *label,
                       bool active);

    bool midi1Out;
    bool midi1In;
    bool midi2Out;
    bool midi2In;
    bool usbHostIo;
    bool usbDevIo;

    bool requestMidi1Out;
    bool requestMidi1In;
    bool requestMidi2Out;
    bool requestMidi2In;
    bool requestUsbHostIo;
    bool requestUsbDevIo;

    bool repaintMidi1Out;
    bool repaintMidi1In;
    bool repaintMidi2Out;
    bool repaintMidi2In;
    bool repaintUsbHostIo;
    bool repaintUsbDevIo;

    bool needsRepaint;
};
