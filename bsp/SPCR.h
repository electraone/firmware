#ifndef SPCR_H
#define SPCR_H

#include "kinetis.h"
#include "core_pins.h"
#include "pins_electra.h"

#ifdef __cplusplus

#define GPIO_BITBAND_ADDR(reg, bit) (((uint32_t)&(reg) - 0x40000000) * 32 + (bit) * 4 + 0x42000000)
#define CONFIG_PULLUP (PORT_PCR_MUX(1) | PORT_PCR_PE | PORT_PCR_PS)
#define CONFIG_NOPULLUP (PORT_PCR_MUX(1))


#define GPIO_SETBIT_ATOMIC(reg, bit) (*(uint32_t *)GPIO_BITBAND_ADDR((reg), (bit)) = 1)
#define GPIO_CLRBIT_ATOMIC(reg, bit) (*(uint32_t *)GPIO_BITBAND_ADDR((reg), (bit)) = 0)

// SPI Control Register ­ SPCR
#define SPIE	7	// SPI Interrupt Enable - not supported
#define SPE	6	// SPI Enable
#define DORD	5	// DORD: Data Order
#define MSTR	4	// MSTR: Master/Slave Select
#define CPOL	3	// CPOL: Clock Polarity
#define CPHA	2	// CPHA: Clock Phase
#define SPR1	1	// Clock: 3 = 125 kHz, 2 = 250 kHz, 1 = 1 MHz, 0->4 MHz
#define SPR0	0
// SPI Status Register ­ SPSR
#define SPIF	7	// SPIF: SPI Interrupt Flag
#define WCOL	6	// WCOL: Write COLlision Flag - not implemented
#define SPI2X	0	// SPI2X: Double SPI Speed Bit
// SPI Data Register ­ SPDR


class SPCRemulation;
class SPSRemulation;
class SPDRemulation;


class SPCRemulation
{
public:
	inline SPCRemulation & operator = (int val) __attribute__((always_inline)) {
		uint32_t ctar, mcr, sim6;
		//serial_print("SPCR=");
		//serial_phex(val);
		//serial_print("\n");
		sim6 = SIM_SCGC6;
		if (!(sim6 & SIM_SCGC6_SPI0)) {
			//serial_print("init1\n");
			SIM_SCGC6 = sim6 | SIM_SCGC6_SPI0;
			SPI0_CTAR0 = SPI_CTAR_FMSZ(7) | SPI_CTAR_PBR(1) | SPI_CTAR_BR(1) | SPI_CTAR_CSSCK(1);
		}
		if (!(val & (1<<SPE))) {
			SPI0_MCR |= SPI_MCR_MDIS; // TODO: use bitband for atomic access
		}
		ctar = SPI_CTAR_FMSZ(7) | SPI_CTAR_PBR(1);
		if (val & (1<<DORD))  ctar |= SPI_CTAR_LSBFE;
		if (val & (1<<CPOL))  ctar |= SPI_CTAR_CPOL;
		if (val & (1<<CPHA)) {
			ctar |= SPI_CTAR_CPHA;
			if ((val & 3) == 0) {
				ctar |= SPI_CTAR_BR(1) | SPI_CTAR_ASC(1);
			} else if ((val & 3) == 1) {
				ctar |= SPI_CTAR_BR(4) | SPI_CTAR_ASC(4);
			} else if ((val & 3) == 2) {
				ctar |= SPI_CTAR_BR(6) | SPI_CTAR_ASC(6);
			} else {
				ctar |= SPI_CTAR_BR(7) | SPI_CTAR_ASC(7);
			}
		} else {
			if ((val & 3) == 0) {
				ctar |= SPI_CTAR_BR(1) | SPI_CTAR_CSSCK(1);
			} else if ((val & 3) == 1) {
				ctar |= SPI_CTAR_BR(4) | SPI_CTAR_CSSCK(4);
			} else if ((val & 3) == 2) {
				ctar |= SPI_CTAR_BR(6) | SPI_CTAR_CSSCK(6);
			} else {
				ctar |= SPI_CTAR_BR(7) | SPI_CTAR_CSSCK(7);
			}
		}
		ctar |= (SPI0_CTAR0 & SPI_CTAR_DBR);
		update_ctar(ctar);
		mcr = SPI_MCR_DCONF(0) | SPI_MCR_PCSIS(0x1F);
		if (val & (1<<MSTR)) mcr |= SPI_MCR_MSTR;
		if (val & (1<<SPE)) {
			mcr &= ~(SPI_MCR_MDIS | SPI_MCR_HALT);
			SPI0_MCR = mcr;
			enable_pins();
		} else {
			mcr |= (SPI_MCR_MDIS | SPI_MCR_HALT);
			SPI0_MCR = mcr;
			disable_pins();
		}
		//serial_print("MCR:");
		//serial_phex32(SPI0_MCR);
		//serial_print(", CTAR0:");
		//serial_phex32(SPI0_CTAR0);
		//serial_print("\n");
		return *this;
	}
	inline SPCRemulation & operator |= (int val) __attribute__((always_inline)) {
		uint32_t sim6;
		//serial_print("SPCR |= ");
		//serial_phex(val);
		//serial_print("\n");
		sim6 = SIM_SCGC6;
		if (!(sim6 & SIM_SCGC6_SPI0)) {
			//serial_print("init2\n");
			SIM_SCGC6 = sim6 | SIM_SCGC6_SPI0;
			SPI0_CTAR0 = SPI_CTAR_FMSZ(7) | SPI_CTAR_PBR(1) | SPI_CTAR_BR(1);
		}
		if (val & ((1<<DORD)|(1<<CPOL)|(1<<CPHA)|3)) {
			uint32_t ctar = SPI0_CTAR0;
			if (val & (1<<DORD)) ctar |= SPI_CTAR_LSBFE; // TODO: use bitband
			if (val & (1<<CPOL)) ctar |= SPI_CTAR_CPOL;
			if (val & (1<<CPHA) && !(ctar & SPI_CTAR_CPHA)) {
				ctar |= SPI_CTAR_CPHA;
				ctar &= 0xFFFF00FF;
				ctar |= SPI_CTAR_ASC(ctar & 15);
			}
			if ((val & 3) != 0) {
				uint32_t br = ctar & 15;
				uint32_t priorval;
				if (br <= 1) priorval = 0;
				else if (br <= 4) priorval = 1;
				else if (br <= 6) priorval = 2;
				else priorval = 3;
				uint32_t newval = priorval | (val & 3);
				if (newval != priorval) {
					if (newval == 0) br = 1;
					else if (newval == 0) br = 4;
					else if (newval == 0) br = 6;
					else br = 7;
					ctar &= 0xFFFF00F0; // clear BR, ASC, CSSCK
					if ((ctar & SPI_CTAR_CPHA)) {
						ctar |= SPI_CTAR_BR(br) | SPI_CTAR_ASC(br);
					} else {
						ctar |= SPI_CTAR_BR(br) | SPI_CTAR_CSSCK(br);
					}
				}
			}
			update_ctar(ctar);
		}
		if (val & (1<<MSTR)) SPI0_MCR |= SPI_MCR_MSTR;
		if (val & (1<<SPE)) {
			SPI0_MCR &= ~(SPI_MCR_MDIS | SPI_MCR_HALT);
			enable_pins();
		}
		//serial_print("MCR:");
		//serial_phex32(SPI0_MCR);
		//serial_print(", CTAR0:");
		//serial_phex32(SPI0_CTAR0);
		//serial_print("\n");
		return *this;
	}
	inline SPCRemulation & operator &= (int val) __attribute__((always_inline)) {
		//serial_print("SPCR &= ");
		//serial_phex(val);
		//serial_print("\n");
		SIM_SCGC6 |= SIM_SCGC6_SPI0;
		if (!(val & (1<<SPE))) {
			SPI0_MCR |= (SPI_MCR_MDIS | SPI_MCR_HALT);
			disable_pins();
		}
		if ((val & ((1<<DORD)|(1<<CPOL)|(1<<CPHA)|3)) != ((1<<DORD)|(1<<CPOL)|(1<<CPHA)|3)) {
			uint32_t ctar = SPI0_CTAR0;
			if (!(val & (1<<DORD))) ctar &= ~SPI_CTAR_LSBFE; // TODO: use bitband
			if (!(val & (1<<CPOL))) ctar &= ~SPI_CTAR_CPOL;
			if (!(val & (1<<CPHA)) && (ctar & SPI_CTAR_CPHA)) {
				ctar &= ~SPI_CTAR_CPHA;
				ctar &= 0xFFFF00FF;
				ctar |= SPI_CTAR_CSSCK(ctar & 15);
			}
			if ((val & 3) != 3) {
				uint32_t br = ctar & 15;
				uint32_t priorval;
				if (br <= 1) priorval = 0;
				else if (br <= 4) priorval = 1;
				else if (br <= 6) priorval = 2;
				else priorval = 3;
				uint32_t newval = priorval & (val & 3);
				if (newval != priorval) {
					if (newval == 0) br = 1;
					else if (newval == 0) br = 4;
					else if (newval == 0) br = 6;
					else br = 7;
					ctar &= 0xFFFF00F0; // clear BR, ASC, CSSCK
					if ((ctar & SPI_CTAR_CPHA)) {
						ctar |= SPI_CTAR_BR(br) | SPI_CTAR_ASC(br);
					} else {
						ctar |= SPI_CTAR_BR(br) | SPI_CTAR_CSSCK(br);
					}
				}
			}
			update_ctar(ctar);
		}
		if (!(val & (1<<MSTR))) SPI0_MCR &= ~SPI_MCR_MSTR;
		return *this;
	}
	inline int operator & (int val) const __attribute__((always_inline)) {
		int ret = 0;
		//serial_print("SPCR & ");
		//serial_phex(val);
		//serial_print("\n");
		SIM_SCGC6 |= SIM_SCGC6_SPI0;
		if ((val & (1<<DORD)) && (SPI0_CTAR0 & SPI_CTAR_LSBFE)) ret |= (1<<DORD);
		if ((val & (1<<CPOL)) && (SPI0_CTAR0 & SPI_CTAR_CPOL)) ret |= (1<<CPOL);
		if ((val & (1<<CPHA)) && (SPI0_CTAR0 & SPI_CTAR_CPHA)) ret |= (1<<CPHA);
		if ((val & 3) != 0) {
			uint32_t dbr = SPI0_CTAR0 & 15;
			uint32_t spr10;
			if (dbr <= 1) {
				spr10 = 0;
			} else if (dbr <= 4) {
				spr10 |= (1<<SPR0);
			} else if (dbr <= 6) {
				spr10 |= (1<<SPR1);
			} else {
				spr10 |= (1<<SPR1)|(1<<SPR0);
			}
			ret |= spr10 & (val & 3);
		}
		if (val & (1<<SPE) && (!(SPI0_MCR & SPI_MCR_MDIS))) ret |= (1<<SPE);
		if (val & (1<<MSTR) && (SPI0_MCR & SPI_MCR_MSTR)) ret |= (1<<MSTR);
		//serial_print("ret = ");
		//serial_phex(ret);
		//serial_print("\n");
		return ret;
	}
	operator int () const __attribute__((always_inline)) {
		int ret = 0;
		if ((SIM_SCGC6 & SIM_SCGC6_SPI0)) {
			int ctar = SPI0_CTAR0;
			if (ctar & SPI_CTAR_LSBFE) ret |= (1<<DORD);
			if (ctar & SPI_CTAR_CPOL) ret |= (1<<CPOL);
			if (ctar & SPI_CTAR_CPHA) ret |= (1<<CPHA);
			ctar &= 15;
			if (ctar <= 1) {
			} else if (ctar <= 4) {
				ret |= (1<<SPR0);
			} else if (ctar <= 6) {
				ret |= (1<<SPR1);
			} else {
				ret |= (1<<SPR1)|(1<<SPR0);
			}
			int mcr = SPI0_MCR;
			if (!(mcr & SPI_MCR_MDIS)) ret |= (1<<SPE);
			if (mcr & SPI_MCR_MSTR) ret |= (1<<MSTR);
		}
		return ret;
	}
	inline void setMOSI(uint8_t pin) __attribute__((always_inline)) {
		uint8_t newpinout = pinout;
		// More than two options so now 2 bits
		if (pin == 11) newpinout &= ~3;
		if (pin == 7) newpinout  =(newpinout & ~0x3) | 1;
		if (pin == 28) newpinout = (newpinout & ~0x3) | 2;
		if ((SIM_SCGC6 & SIM_SCGC6_SPI0) && newpinout != pinout) {
			// First unconfigure previous pin
			switch (pinout & 3) {
				case 0: CORE_PIN11_CONFIG = PORT_PCR_SRE | PORT_PCR_DSE | PORT_PCR_MUX(1); break;
				case 1: CORE_PIN7_CONFIG = PORT_PCR_SRE | PORT_PCR_DSE | PORT_PCR_MUX(1); break;
				default: CORE_PIN28_CONFIG = PORT_PCR_SRE | PORT_PCR_DSE | PORT_PCR_MUX(1);
			}
			switch (newpinout & 3) {
				case 0: CORE_PIN11_CONFIG = PORT_PCR_DSE | PORT_PCR_MUX(2); break;
				case 1: CORE_PIN7_CONFIG = PORT_PCR_MUX(2); break;
				default: CORE_PIN28_CONFIG = PORT_PCR_MUX(2);
			}
		}
		pinout = newpinout;
	}
	inline void setMOSI_soft(uint8_t pin) __attribute__((always_inline)) {
		if (pin == 11) pinout &= ~3;
		if (pin == 7)  pinout = (pinout & ~0x3) | 1;
		if (pin == 28) pinout = (pinout & ~0x3) | 2;
	}
	inline void setMISO(uint8_t pin) __attribute__((always_inline)) {
		uint8_t newpinout = pinout;
		// More than two options so now 2 bits
		if (pin == 12) newpinout &= ~0xc;
		if (pin == 8) newpinout  =(newpinout & ~0xc) | 4;
		if (pin == 39) newpinout = (newpinout & ~0xc) | 8;
		if ((SIM_SCGC6 & SIM_SCGC6_SPI0) && newpinout != pinout) {
			// First unconfigure previous pin
			switch (pinout & 0xc) {
				case 0: CORE_PIN12_CONFIG = PORT_PCR_SRE | PORT_PCR_DSE | PORT_PCR_MUX(1); break;
				case 0x4: CORE_PIN8_CONFIG = PORT_PCR_SRE | PORT_PCR_DSE | PORT_PCR_MUX(1); break;
				default: CORE_PIN39_CONFIG = PORT_PCR_SRE | PORT_PCR_DSE | PORT_PCR_MUX(1);
			}
			switch (newpinout & 0xc) {
				case 0: CORE_PIN12_CONFIG = PORT_PCR_MUX(2); break;
				case 0x4: CORE_PIN8_CONFIG = PORT_PCR_MUX(2); break;
				default: CORE_PIN39_CONFIG = PORT_PCR_MUX(2);
			}
		}
		pinout = newpinout;
	}
	inline void setMISO_soft(uint8_t pin) __attribute__((always_inline)) {
		if (pin == 12) pinout &= ~0xc;
		if (pin == 8)  pinout = (pinout & ~0xc) | 4;
		if (pin == 39) pinout = (pinout & ~0xc) | 8;
	}
	inline void setSCK(uint8_t pin) __attribute__((always_inline)) {
		uint8_t newpinout = pinout;
		// More than two options so now 2 bits
		if (pin == 13) newpinout &= ~0x30;
		if (pin == 14) newpinout  =(newpinout & ~0x30) | 0x10;
		if (pin == 27) newpinout = (newpinout & ~0x30) | 0x20;
		if ((SIM_SCGC6 & SIM_SCGC6_SPI0) && newpinout != pinout) {
			// First unconfigure previous pin
			switch (pinout & 0x30) {
				case 0: CORE_PIN13_CONFIG = PORT_PCR_SRE | PORT_PCR_DSE | PORT_PCR_MUX(1); break;
				case 0x10: CORE_PIN14_CONFIG = PORT_PCR_SRE | PORT_PCR_DSE | PORT_PCR_MUX(1); break;
				default: CORE_PIN27_CONFIG = PORT_PCR_SRE | PORT_PCR_DSE | PORT_PCR_MUX(1);
			}
			switch (newpinout & 0x30) {
				case 0: CORE_PIN13_CONFIG =  PORT_PCR_DSE | PORT_PCR_MUX(2); break;
				case 0x10: CORE_PIN14_CONFIG = PORT_PCR_MUX(2); break;
				default: CORE_PIN27_CONFIG = PORT_PCR_MUX(2);
			}
		}
		pinout = newpinout;
	}
	inline void setSCK_soft(uint8_t pin) __attribute__((always_inline)) {
		if (pin == 13) pinout &= ~0x30;
		if (pin == 14) pinout = (pinout & ~0x30) | 0x10;
		if (pin == 27) pinout = (pinout & ~0x30) | 0x20;
	}
	friend class SPSRemulation;
	friend class SPIFIFOclass;
private:
	static inline void update_ctar(uint32_t ctar) __attribute__((always_inline)) {
		if (SPI0_CTAR0 == ctar) return;
		uint32_t mcr = SPI0_MCR;
		if (mcr & SPI_MCR_MDIS) {
			SPI0_CTAR0 = ctar;
		} else {
			SPI0_MCR = mcr | SPI_MCR_MDIS | SPI_MCR_HALT;
			SPI0_CTAR0 = ctar;
			SPI0_MCR = mcr;
		}
	}
	static uint8_t pinout;
public:
	inline void enable_pins(void) __attribute__((always_inline)) {
		switch (pinout & 3) {
			case 0: CORE_PIN11_CONFIG = PORT_PCR_DSE | PORT_PCR_MUX(2); break;
			case 1: CORE_PIN7_CONFIG = PORT_PCR_MUX(2); break;
			default: CORE_PIN28_CONFIG = PORT_PCR_MUX(2);
		}
		switch (pinout & 0xc) {
			case 0: CORE_PIN12_CONFIG = PORT_PCR_MUX(2); break;
			case 0x4: CORE_PIN8_CONFIG = PORT_PCR_MUX(2); break;
			default: CORE_PIN39_CONFIG = PORT_PCR_MUX(2);
		}
		switch (pinout & 0x30) {
			case 0: CORE_PIN13_CONFIG =  PORT_PCR_DSE | PORT_PCR_MUX(2); break;
			case 0x10: CORE_PIN14_CONFIG = PORT_PCR_MUX(2); break;
			default: CORE_PIN27_CONFIG = PORT_PCR_MUX(2);
		}
	}
	inline void disable_pins(void) __attribute__((always_inline)) {
		switch (pinout & 3) {
			case 0: CORE_PIN11_CONFIG = PORT_PCR_SRE | PORT_PCR_DSE | PORT_PCR_MUX(1); break;
			case 1: CORE_PIN7_CONFIG = PORT_PCR_SRE | PORT_PCR_DSE | PORT_PCR_MUX(1); break;
			default: CORE_PIN28_CONFIG = PORT_PCR_SRE | PORT_PCR_DSE | PORT_PCR_MUX(1);
		}

		switch (pinout & 0xc) {
			case 0: CORE_PIN12_CONFIG = PORT_PCR_SRE | PORT_PCR_DSE | PORT_PCR_MUX(1); break;
			case 0x4: CORE_PIN8_CONFIG = PORT_PCR_SRE | PORT_PCR_DSE | PORT_PCR_MUX(1); break;
			default: CORE_PIN39_CONFIG = PORT_PCR_SRE | PORT_PCR_DSE | PORT_PCR_MUX(1);
		}
		switch (pinout & 0x30) {
			case 0: CORE_PIN13_CONFIG = PORT_PCR_SRE | PORT_PCR_DSE | PORT_PCR_MUX(1); break;
			case 0x10: CORE_PIN14_CONFIG = PORT_PCR_SRE | PORT_PCR_DSE | PORT_PCR_MUX(1); break;
			default: CORE_PIN27_CONFIG = PORT_PCR_SRE | PORT_PCR_DSE | PORT_PCR_MUX(1);
		}
	}
};

extern SPCRemulation SPCR;

#endif // __cplusplus

#endif // SPCR_H
