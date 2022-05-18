#pragma once

#include "helpers.h"

// #define MUX_ENABLE  GPIOB_PDOR = GPIOB_PDOR | 0x10; delayMicroseconds(15);
// #define MUX_DISABLE GPIOB_PDOR = 0x00;

#define MUX_ENABLE                                                             \
    digitalWriteFast(49, LOW);                                                 \
    delayMicroseconds(1);
#define MUX_DISABLE digitalWriteFast(49, HIGH)

void configureMux(void);
void setMuxAddress(uint8_t address);
