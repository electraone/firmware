#ifndef SPI_DRIVER
#define SPI_DRIVER

#include <Arduino.h>

#ifndef LSBFIRST
#define LSBFIRST 0
#endif
#ifndef MSBFIRST
#define MSBFIRST 1
#endif

#define SPI_MODE0 0x00
#define SPI_MODE1 0x04
#define SPI_MODE2 0x08
#define SPI_MODE3 0x0C

#define SPI_CLOCK_DIV4 0x00
#define SPI_CLOCK_DIV16 0x01
#define SPI_CLOCK_DIV64 0x02
#define SPI_CLOCK_DIV128 0x03
#define SPI_CLOCK_DIV2 0x04
#define SPI_CLOCK_DIV8 0x05
#define SPI_CLOCK_DIV32 0x06

#define SPI_MODE_MASK 0x0C // CPOL = bit 3, CPHA = bit 2 on SPCR
#define SPI_CLOCK_MASK 0x03 // SPR1 = bit 1, SPR0 = bit 0 on SPCR
#define SPI_2XCLOCK_MASK 0x01 // SPI2X = bit 0 on SPSR

class SPISettings
{
public:
    SPISettings(uint32_t clock, uint8_t bitOrder, uint8_t dataMode)
    {
        if (__builtin_constant_p(clock)) {
            init_AlwaysInline(clock, bitOrder, dataMode);
        } else {
            init_MightInline(clock, bitOrder, dataMode);
        }
    }
    SPISettings()
    {
        init_AlwaysInline(4000000, MSBFIRST, SPI_MODE0);
    }

private:
    void init_MightInline(uint32_t clock, uint8_t bitOrder, uint8_t dataMode)
    {
        init_AlwaysInline(clock, bitOrder, dataMode);
    }
    void init_AlwaysInline(uint32_t clock, uint8_t bitOrder, uint8_t dataMode)
        __attribute__((__always_inline__))
    {
        uint32_t t, c = SPI_CTAR_FMSZ(7);
        if (bitOrder == LSBFIRST)
            c |= SPI_CTAR_LSBFE;
        if (__builtin_constant_p(clock)) {
            if (clock >= F_BUS / 2) {
                t = SPI_CTAR_PBR(0) | SPI_CTAR_BR(0) | SPI_CTAR_DBR
                    | SPI_CTAR_CSSCK(0);
            } else if (clock >= F_BUS / 3) {
                t = SPI_CTAR_PBR(1) | SPI_CTAR_BR(0) | SPI_CTAR_DBR
                    | SPI_CTAR_CSSCK(0);
            } else if (clock >= F_BUS / 4) {
                t = SPI_CTAR_PBR(0) | SPI_CTAR_BR(0) | SPI_CTAR_CSSCK(0);
            } else if (clock >= F_BUS / 5) {
                t = SPI_CTAR_PBR(2) | SPI_CTAR_BR(0) | SPI_CTAR_DBR
                    | SPI_CTAR_CSSCK(0);
            } else if (clock >= F_BUS / 6) {
                t = SPI_CTAR_PBR(1) | SPI_CTAR_BR(0) | SPI_CTAR_CSSCK(0);
            } else if (clock >= F_BUS / 8) {
                t = SPI_CTAR_PBR(0) | SPI_CTAR_BR(1) | SPI_CTAR_CSSCK(1);
            } else if (clock >= F_BUS / 10) {
                t = SPI_CTAR_PBR(2) | SPI_CTAR_BR(0) | SPI_CTAR_CSSCK(0);
            } else if (clock >= F_BUS / 12) {
                t = SPI_CTAR_PBR(1) | SPI_CTAR_BR(1) | SPI_CTAR_CSSCK(1);
            } else if (clock >= F_BUS / 16) {
                t = SPI_CTAR_PBR(0) | SPI_CTAR_BR(3) | SPI_CTAR_CSSCK(2);
            } else if (clock >= F_BUS / 20) {
                t = SPI_CTAR_PBR(2) | SPI_CTAR_BR(1) | SPI_CTAR_CSSCK(0);
            } else if (clock >= F_BUS / 24) {
                t = SPI_CTAR_PBR(1) | SPI_CTAR_BR(3) | SPI_CTAR_CSSCK(2);
            } else if (clock >= F_BUS / 32) {
                t = SPI_CTAR_PBR(0) | SPI_CTAR_BR(4) | SPI_CTAR_CSSCK(3);
            } else if (clock >= F_BUS / 40) {
                t = SPI_CTAR_PBR(2) | SPI_CTAR_BR(3) | SPI_CTAR_CSSCK(2);
            } else if (clock >= F_BUS / 56) {
                t = SPI_CTAR_PBR(3) | SPI_CTAR_BR(3) | SPI_CTAR_CSSCK(2);
            } else if (clock >= F_BUS / 64) {
                t = SPI_CTAR_PBR(0) | SPI_CTAR_BR(5) | SPI_CTAR_CSSCK(4);
            } else if (clock >= F_BUS / 96) {
                t = SPI_CTAR_PBR(1) | SPI_CTAR_BR(5) | SPI_CTAR_CSSCK(4);
            } else if (clock >= F_BUS / 128) {
                t = SPI_CTAR_PBR(0) | SPI_CTAR_BR(6) | SPI_CTAR_CSSCK(5);
            } else if (clock >= F_BUS / 192) {
                t = SPI_CTAR_PBR(1) | SPI_CTAR_BR(6) | SPI_CTAR_CSSCK(5);
            } else if (clock >= F_BUS / 256) {
                t = SPI_CTAR_PBR(0) | SPI_CTAR_BR(7) | SPI_CTAR_CSSCK(6);
            } else if (clock >= F_BUS / 384) {
                t = SPI_CTAR_PBR(1) | SPI_CTAR_BR(7) | SPI_CTAR_CSSCK(6);
            } else if (clock >= F_BUS / 512) {
                t = SPI_CTAR_PBR(0) | SPI_CTAR_BR(8) | SPI_CTAR_CSSCK(7);
            } else if (clock >= F_BUS / 640) {
                t = SPI_CTAR_PBR(2) | SPI_CTAR_BR(7) | SPI_CTAR_CSSCK(6);
            } else { /* F_BUS / 768 */
                t = SPI_CTAR_PBR(1) | SPI_CTAR_BR(8) | SPI_CTAR_CSSCK(7);
            }
        } else {
            for (uint32_t i = 0; i < 23; i++) {
                t = ctar_clock_table[i];
                if (clock >= F_BUS / ctar_div_table[i])
                    break;
            }
        }
        if (dataMode & 0x08) {
            c |= SPI_CTAR_CPOL;
        }
        if (dataMode & 0x04) {
            c |= SPI_CTAR_CPHA;
            t = (t & 0xFFFF0FFF) | ((t & 0xF000) >> 4);
        }
        ctar = c | t;
    }
    static const uint16_t ctar_div_table[23];
    static const uint32_t ctar_clock_table[23];
    uint32_t ctar;
    friend class SPIClass;
};

class SPIClass
{
public:
    static const uint8_t CNT_MISO_PINS = 4;
    static const uint8_t CNT_MOSI_PINS = 4;
    static const uint8_t CNT_SCK_PINS = 3;
    static const uint8_t CNT_CS_PINS = 11;

    typedef struct {
        volatile uint32_t &clock_gate_register;
        uint32_t clock_gate_mask;
        uint8_t queue_size;
        uint8_t spi_irq;
        uint32_t max_dma_count;
        uint8_t tx_dma_channel;
        uint8_t rx_dma_channel;
        void (*dma_rxisr)();
        uint8_t miso_pin[CNT_MISO_PINS];
        uint32_t miso_mux[CNT_MISO_PINS];
        uint8_t mosi_pin[CNT_MOSI_PINS];
        uint32_t mosi_mux[CNT_MOSI_PINS];
        uint8_t sck_pin[CNT_SCK_PINS];
        uint32_t sck_mux[CNT_SCK_PINS];
        uint8_t cs_pin[CNT_CS_PINS];
        uint32_t cs_mux[CNT_CS_PINS];
        uint8_t cs_mask[CNT_CS_PINS];
    } SPI_Hardware_t;
    static const SPI_Hardware_t spi0_hardware;
    static const SPI_Hardware_t spi1_hardware;
    static const SPI_Hardware_t spi2_hardware;

    enum DMAState { notAllocated, idle, active, completed };

public:
    constexpr SPIClass(uintptr_t myport, uintptr_t myhardware)
        : port_addr(myport), hardware_addr(myhardware)
    {
    }
    // Initialize the SPI library
    void begin();

    // Before using SPI.transfer() or asserting chip select pins,
    // this function is used to gain exclusive access to the SPI bus
    // and configure the correct settings.
    void beginTransaction(SPISettings settings)
    {
        if (port().CTAR0 != settings.ctar) {
            port().MCR = SPI_MCR_MDIS | SPI_MCR_HALT | SPI_MCR_PCSIS(0x3F);
            port().CTAR0 = settings.ctar;
            port().CTAR1 = settings.ctar | SPI_CTAR_FMSZ(8);
            port().MCR = SPI_MCR_MSTR | SPI_MCR_PCSIS(0x3F);
        }
    }

    // Write to the SPI bus (MOSI pin) and also receive (MISO pin)
    uint8_t transfer(uint8_t data)
    {
        port().SR = SPI_SR_TCF;
        port().PUSHR = data;
        while (!(port().SR & SPI_SR_TCF))
            ; // wait
        return port().POPR;
    }
    uint16_t transfer16(uint16_t data)
    {
        port().SR = SPI_SR_TCF;
        port().PUSHR = data | SPI_PUSHR_CTAS(1);
        while (!(port().SR & SPI_SR_TCF))
            ; // wait
        return port().POPR;
    }

    void inline transfer(void *buf, size_t count)
    {
        transfer(buf, buf, count);
    }
    void setTransferWriteFill(uint8_t ch)
    {
        _transferWriteFill = ch;
    }
    void transfer(const void *buf, void *retbuf, size_t count);

    // After performing a group of transfers and releasing the chip select
    // signal, this function allows others to access the SPI bus
    void endTransaction(void)
    {
    }

    // Disable the SPI bus
    void end();

    // This function is deprecated.	 New applications should use
    // beginTransaction() to configure SPI settings.
    void setBitOrder(uint8_t bitOrder);

    // This function is deprecated.	 New applications should use
    // beginTransaction() to configure SPI settings.
    void setDataMode(uint8_t dataMode);

    // This function is deprecated.	 New applications should use
    // beginTransaction() to configure SPI settings.
    void setClockDivider(uint8_t clockDiv)
    {
        if (clockDiv == SPI_CLOCK_DIV2) {
            setClockDivider_noInline(
                SPISettings(12000000, MSBFIRST, SPI_MODE0).ctar);
        } else if (clockDiv == SPI_CLOCK_DIV4) {
            setClockDivider_noInline(
                SPISettings(4000000, MSBFIRST, SPI_MODE0).ctar);
        } else if (clockDiv == SPI_CLOCK_DIV8) {
            setClockDivider_noInline(
                SPISettings(2000000, MSBFIRST, SPI_MODE0).ctar);
        } else if (clockDiv == SPI_CLOCK_DIV16) {
            setClockDivider_noInline(
                SPISettings(1000000, MSBFIRST, SPI_MODE0).ctar);
        } else if (clockDiv == SPI_CLOCK_DIV32) {
            setClockDivider_noInline(
                SPISettings(500000, MSBFIRST, SPI_MODE0).ctar);
        } else if (clockDiv == SPI_CLOCK_DIV64) {
            setClockDivider_noInline(
                SPISettings(250000, MSBFIRST, SPI_MODE0).ctar);
        } else { /* clockDiv == SPI_CLOCK_DIV128 */
            setClockDivider_noInline(
                SPISettings(125000, MSBFIRST, SPI_MODE0).ctar);
        }
    }
    void setClockDivider_noInline(uint32_t clk);

    // These undocumented functions should not be used.  SPI.transfer()
    // polls the hardware flag which is automatically cleared as the
    // AVR responds to SPI's interrupt
    void attachInterrupt()
    {
    }
    void detachInterrupt()
    {
    }

    // Teensy 3.x can use alternate pins for these 3 SPI signals.
    void setMOSI(uint8_t pin);
    void setMISO(uint8_t pin);
    void setSCK(uint8_t pin);

    // return true if "pin" has special chip select capability
    uint8_t pinIsChipSelect(uint8_t pin);
    bool pinIsMOSI(uint8_t pin);
    bool pinIsMISO(uint8_t pin);
    bool pinIsSCK(uint8_t pin);
    // return true if both pin1 and pin2 have independent chip select capability
    bool pinIsChipSelect(uint8_t pin1, uint8_t pin2);
    // configure a pin for chip select and return its SPI_MCR_PCSIS bitmask
    // setCS() is a special function, not intended for use from normal Arduino
    // programs/sketches.  See the ILI3941_t3 library for an example.
    uint8_t setCS(uint8_t pin);

private:
    KINETISK_SPI_t &port()
    {
        return *(KINETISK_SPI_t *)port_addr;
    }
    const SPI_Hardware_t &hardware()
    {
        return *(const SPI_Hardware_t *)hardware_addr;
    }
    void updateCTAR(uint32_t ctar);
    uintptr_t port_addr;
    uintptr_t hardware_addr;
    uint8_t miso_pin_index = 0;
    uint8_t mosi_pin_index = 0;
    uint8_t sck_pin_index = 0;
    uint8_t _transferWriteFill = 0;
};

extern SPIClass SPI;
extern SPIClass SPI1;

#endif // SPI_DRIVER
