#pragma once

#include <stdint.h>

// A0-A9 are always digital 14-23, for Arduino compatibility
#define PIN_A0  (14)
#define PIN_A1  (15)
#define PIN_A2  (16)
#define PIN_A3  (17)
#define PIN_A4  (18)
#define PIN_A5  (19)
#define PIN_A6  (20)
#define PIN_A7  (21)
#define PIN_A8  (22)
#define PIN_A9  (23)
const static uint8_t A0 = PIN_A0;
const static uint8_t A1 = PIN_A1;
const static uint8_t A2 = PIN_A2;
const static uint8_t A3 = PIN_A3;
const static uint8_t A4 = PIN_A4;
const static uint8_t A5 = PIN_A5;
const static uint8_t A6 = PIN_A6;
const static uint8_t A7 = PIN_A7;
const static uint8_t A8 = PIN_A8;
const static uint8_t A9 = PIN_A9;

#define PIN_A10 (64)
#define PIN_A11 (65)
#define PIN_A12 (31)
#define PIN_A13 (32)
#define PIN_A14 (33)
#define PIN_A15 (34)
#define PIN_A16 (35)
#define PIN_A17 (36)
#define PIN_A18 (37)
#define PIN_A19 (38)
#define PIN_A20 (39)
#define PIN_A21 (66)
#define PIN_A22 (67)
#define PIN_A23 (49)
#define PIN_A24 (50)
#define PIN_A25 (68)
#define PIN_A26 (69)
const static uint8_t A10 = PIN_A10;
const static uint8_t A11 = PIN_A11;
const static uint8_t A12 = PIN_A12;
const static uint8_t A13 = PIN_A13;
const static uint8_t A14 = PIN_A14;
const static uint8_t A15 = PIN_A15;
const static uint8_t A16 = PIN_A16;
const static uint8_t A17 = PIN_A17;
const static uint8_t A18 = PIN_A18;
const static uint8_t A19 = PIN_A19;
const static uint8_t A20 = PIN_A20;
const static uint8_t A21 = PIN_A21;
const static uint8_t A22 = PIN_A22;
const static uint8_t A23 = PIN_A23;
const static uint8_t A24 = PIN_A24;
const static uint8_t A25 = PIN_A25;
const static uint8_t A26 = PIN_A26;

#define LED_BUILTIN   (13)

#define PIN_SPI_SS    (10)
#define PIN_SPI_MOSI  (11)
#define PIN_SPI_MISO  (12)
#define PIN_SPI_SCK   (13)
const static uint8_t SS = 10;
const static uint8_t MOSI = 11;
const static uint8_t MISO = 12;
const static uint8_t SCK = 13;

#define PIN_WIRE_SDA  (18)
#define PIN_WIRE_SCL  (19)
const static uint8_t SDA = 18;
const static uint8_t SCL = 19;

#define PIN_SERIAL_RX (0)
#define PIN_SERIAL_TX (1)


#define NUM_DIGITAL_PINS  CORE_NUM_DIGITAL
#define NUM_ANALOG_INPUTS CORE_NUM_ANALOG


#define NOT_AN_INTERRUPT -1

// TODO analogInputToDigitalPin needs update...
#define analogInputToDigitalPin(p) (((p) <= 9) ? (p) + 14 : (((p) >= 12 && (p) <= 20) ? (p) + 19 : -1))
#define digitalPinHasPWM(p) (((p) >= 2 && (p) <= 10) || (p) == 14 || ((p) >= 20 && (p) <= 23) || (p) == 29 || (p) == 30 || ((p) >= 35 && (p) <= 38))
#define digitalPinToInterrupt(p)  ((p) < NUM_DIGITAL_PINS ? (p) : -1)

#define digitalPinToPCICR(p)    ((volatile uint8_t *)0)
#define digitalPinToPCICRbit(p) (0)
#define digitalPinToPCIFR(p)    ((volatile uint8_t *)0)
#define digitalPinToPCIFRbit(p) (0)
#define digitalPinToPCMSK(p)    ((volatile uint8_t *)0)
#define digitalPinToPCMSKbit(p) (0)


#if defined(KINETISK)
struct digital_pin_bitband_and_config_table_struct {
        volatile uint32_t *reg;
        volatile uint32_t *config;
};
extern const struct digital_pin_bitband_and_config_table_struct digital_pin_to_info_PGM[];
// compatibility macros
#define digitalPinToPort(pin) (pin)
#define digitalPinToBitMask(pin) (1)
#define portOutputRegister(pin) ((volatile uint8_t *)(digital_pin_to_info_PGM[(pin)].reg + 0))
#define portSetRegister(pin)    ((volatile uint8_t *)(digital_pin_to_info_PGM[(pin)].reg + 32))
#define portClearRegister(pin)  ((volatile uint8_t *)(digital_pin_to_info_PGM[(pin)].reg + 64))
#define portToggleRegister(pin) ((volatile uint8_t *)(digital_pin_to_info_PGM[(pin)].reg + 96))
#define portInputRegister(pin)  ((volatile uint8_t *)(digital_pin_to_info_PGM[(pin)].reg + 128))
#define portModeRegister(pin)   ((volatile uint8_t *)(digital_pin_to_info_PGM[(pin)].reg + 160))
#define portConfigRegister(pin) ((volatile uint32_t *)(digital_pin_to_info_PGM[(pin)].config))
#define digitalPinToPortReg(pin) (portOutputRegister(pin))
#define digitalPinToBit(pin) (1)

#elif defined(KINETISL)
struct digital_pin_bitband_and_config_table_struct {
        volatile uint8_t *reg;
        volatile uint32_t *config;
	uint8_t mask;
};
extern const struct digital_pin_bitband_and_config_table_struct digital_pin_to_info_PGM[];
// compatibility macros
#define digitalPinToPort(pin) (pin)
#define digitalPinToBitMask(pin) (digital_pin_to_info_PGM[(pin)].mask)
#define portOutputRegister(pin) ((digital_pin_to_info_PGM[(pin)].reg + 0))
#define portSetRegister(pin)    ((digital_pin_to_info_PGM[(pin)].reg + 4))
#define portClearRegister(pin)  ((digital_pin_to_info_PGM[(pin)].reg + 8))
#define portToggleRegister(pin) ((digital_pin_to_info_PGM[(pin)].reg + 12))
#define portInputRegister(pin)  ((digital_pin_to_info_PGM[(pin)].reg + 16))
#define portModeRegister(pin)   ((digital_pin_to_info_PGM[(pin)].reg + 20))
#define portConfigRegister(pin) ((digital_pin_to_info_PGM[(pin)].config))
#define digitalPinToPortReg(pin) (portOutputRegister(pin))
//#define digitalPinToBit(pin) (1)

#endif



#define NOT_ON_TIMER 0
static inline uint8_t digitalPinToTimer(uint8_t) __attribute__((always_inline, unused));
static inline uint8_t digitalPinToTimer(uint8_t pin)
{
	if (pin >= 3 && pin <= 6) return pin - 2;
	if (pin >= 9 && pin <= 10) return pin - 4;
	if (pin >= 20 && pin <= 23) return pin - 13;
	return NOT_ON_TIMER;
}

// These serial port names are intended to allow libraries and architecture-neutral
// sketches to automatically default to the correct port name for a particular type
// of use. For example, a GPS module would normally connect to SERIAL_PORT_HARDWARE_OPEN,
// the first hardware serial port whose RX/TX pins are not dedicated to another use.
//
// SERIAL_PORT_MONITOR Port which normally prints to the Arduino Serial Monitor
//
// SERIAL_PORT_USBVIRTUAL Port which is USB virtual serial
//
// SERIAL_PORT_LINUXBRIDGE Port which connects to a Linux system via Bridge library
//
// SERIAL_PORT_HARDWARE Hardware serial port, physical RX & TX pins.
//
// SERIAL_PORT_HARDWARE_OPEN Hardware serial ports which are open for use. Their RX & TX
// pins are NOT connected to anything by default.
//
#define SERIAL_PORT_MONITOR		Serial
#define SERIAL_PORT_USBVIRTUAL		Serial
#define SERIAL_PORT_HARDWARE		Serial1
#define SERIAL_PORT_HARDWARE1		Serial2
#define SERIAL_PORT_HARDWARE_OPEN	Serial1
#define SERIAL_PORT_HARDWARE_OPEN1	Serial2


#define SerialUSB			Serial
