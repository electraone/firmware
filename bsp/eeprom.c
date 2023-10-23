#include "mk66fx1m0.h"
#include <eeprom.h>

#if F_CPU > 120000000 && defined(__MK66FX1M0__)
#include "core_pins.h"	// delayMicroseconds()
#endif



#define EEPROM_MAX  4096
#define EEPARTITION 0x05  // 128K dataflash for EEPROM, 128K for Data
#define EEESPLIT    0x10  // best endurance: 0x00 = first 12%, 0x10 = first 25%, 0x30 = all equal

#if E2END > (EEPROM_MAX-1)
#error "E2END is set larger than the maximum possible EEPROM size"
#endif

// The EEPROM is really RAM with a hardware-based backup system to
// flash memory.  Selecting a smaller size EEPROM allows more wear
// leveling, for higher write endurance.  If you edit this file,
// set this to the smallest size your application can use.  Also,
// due to Freescale's implementation, writing 16 or 32 bit words
// (aligned to 2 or 4 byte boundaries) has twice the endurance
// compared to writing 8 bit bytes.
//

#define EEPROM_SIZE 4096
#define EEESIZE 0x02


// Writing unaligned 16 or 32 bit data is handled automatically when
// this is defined, but at a cost of extra code size.  Without this,
// any unaligned write will cause a hard fault exception!  If you're
// absolutely sure all 16 and 32 bit writes will be aligned, you can
// remove the extra unnecessary code.
//
#define HANDLE_UNALIGNED_WRITES


void eeprom_initialize(void)
{
	uint32_t count=0;
	uint16_t do_flash_cmd[] = {
		0xf06f, 0x037f, 0x7003, 0x7803,
		0xf013, 0x0f80, 0xd0fb, 0x4770};
	uint8_t status;

	if (FTFL_FCNFG & FTFL_FCNFG_RAMRDY) {
		uint8_t stat = FTFL_FSTAT & 0x70;
		if (stat) FTFL_FSTAT = stat;
		// FlexRAM is configured as traditional RAM
		// We need to reconfigure for EEPROM usage
		kinetis_hsrun_disable();
		FTFL_FCCOB0 = 0x80; // PGMPART = Program Partition Command
		FTFL_FCCOB3 = 0;
		FTFL_FCCOB4 = EEESPLIT | EEESIZE;
		FTFL_FCCOB5 = EEPARTITION;
		__disable_irq();
		// do_flash_cmd() must execute from RAM.  Luckily the C syntax is simple...
		(*((void (*)(volatile uint8_t *))((uint32_t)do_flash_cmd | 1)))(&FTFL_FSTAT);
		__enable_irq();
		kinetis_hsrun_enable();
		status = FTFL_FSTAT;
		if (status & 0x70) {
			FTFL_FSTAT = (status & 0x70);
			return; // error
		}
	}
	// wait for eeprom to become ready (is this really necessary?)
	while (!(FTFL_FCNFG & FTFL_FCNFG_EEERDY)) {
		if (++count > 200000) break;
	}
}

#define FlexRAM ((volatile uint8_t *)0x14000000)

uint8_t eeprom_read_byte(const uint8_t *addr)
{
	uint32_t offset = (uint32_t)addr;
	if (offset >= EEPROM_SIZE) return 0;
	if (!(FTFL_FCNFG & FTFL_FCNFG_EEERDY)) eeprom_initialize();
	return FlexRAM[offset];
}

int eeprom_is_ready(void)
{
	return (FTFL_FCNFG & FTFL_FCNFG_EEERDY) ? 1 : 0;
}

static void flexram_wait(void)
{
	while (!(FTFL_FCNFG & FTFL_FCNFG_EEERDY)) {
		// TODO: timeout
	}
}

void eeprom_write_byte(uint8_t *addr, uint8_t value)
{
	uint32_t offset = (uint32_t)addr;

	if (offset >= EEPROM_SIZE) return;
	if (!(FTFL_FCNFG & FTFL_FCNFG_EEERDY)) eeprom_initialize();
	if (FlexRAM[offset] != value) {
		kinetis_hsrun_disable();
		uint8_t stat = FTFL_FSTAT & 0x70;
		if (stat) FTFL_FSTAT = stat;
		FlexRAM[offset] = value;
		flexram_wait();
		kinetis_hsrun_enable();
	}
}