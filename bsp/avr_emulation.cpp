#include <Arduino.h>
#include "SPIFIFO.h"

uint8_t SPCRemulation::pinout = 0;
SPCRemulation SPCR;

#if defined(KINETISL)
uint8_t SPCR1emulation::pinout = 0;
SPCR1emulation SPCR1;
#endif

#if defined(__MK64FX512__) || defined(__MK66FX1M0__)
uint8_t SPCR1emulation::pinout = 0;
SPCR1emulation SPCR1;
uint8_t SPCR2emulation::pinout = 0;
SPCR2emulation SPCR2;
#endif

#ifdef HAS_SPIFIFO

uint8_t SPIFIFOclass::pcs = 0;
volatile uint8_t * SPIFIFOclass::reg = 0;

#endif
