#ifndef WProgram_h
#define WProgram_h

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <pgmspace.h>

#include "avr_functions.h"
#include "wiring.h"
#include "HardwareSerial.h"

#define DMAMEM __attribute__ ((section(".dmabuffers"), used))
#define FASTRUN __attribute__ ((section(".fastrun"), noinline, noclone ))

#ifdef __cplusplus

#include "avr_emulation.h"
#include "usb_serial.h"
#include "usb_midi.h"
#include "usb_keyboard.h"
#include "usb_mouse.h"
#include "usb_undef.h" // do not allow usb_desc.h stuff to leak to user programs

#include "WCharacter.h"
#include "WString.h"
#include "elapsedMillis.h"
#include "IntervalTimer.h"

unsigned long pulseIn(uint8_t pin, uint8_t state, unsigned long timeout = 1000000L);

#include "pins_arduino.h"

#endif // __cplusplus


// Fast memcpy
#ifdef __cplusplus
extern "C" {
extern void *memcpy (void *dst, const void *src, size_t count);
}
#endif


#endif // WProgram_h
