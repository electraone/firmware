#pragma once

#include <stdint.h>

void handleMidiIoSysExPort0(const uint8_t *sysExData,
                            uint16_t sysExSize,
                            bool complete);
void handleMidiIoSysExPort1(const uint8_t *sysExData,
                            uint16_t sysExSize,
                            bool complete);
void handleMidiUsbDevSysEx(uint8_t cable,
                           const uint8_t *sysExData,
                           uint16_t sysExSize,
                           bool complete);
void handleMidiUsbHostSysExPort0(uint8_t cable,
                                 const uint8_t *sysExData,
                                 uint16_t sysExSize,
                                 bool complete);
void handleMidiUsbHostSysExPort1(uint8_t cable,
                                 const uint8_t *sysExData,
                                 uint16_t sysExSize,
                                 bool complete);
