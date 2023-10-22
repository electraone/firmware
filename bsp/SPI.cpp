#include "SPI.h"

void _spi_dma_rxISR0(void)
{
    ;
}
void _spi_dma_rxISR1(void)
{
    ;
}
void _spi_dma_rxISR2(void)
{
    ;
}

const SPIClass::SPI_Hardware_t SPIClass::spi0_hardware = {
    SIM_SCGC6,
    SIM_SCGC6_SPI0,
    4,
    IRQ_SPI0,
    32767,
    DMAMUX_SOURCE_SPI0_TX,
    DMAMUX_SOURCE_SPI0_RX,
    _spi_dma_rxISR0,
    12,
    8,
    39,
    255,
    PORT_PCR_MUX(2),
    PORT_PCR_MUX(2),
    PORT_PCR_MUX(2),
    0,
    11,
    7,
    28,
    255,
    PORT_PCR_MUX(2),
    PORT_PCR_MUX(2),
    PORT_PCR_MUX(2),
    0,
    13,
    14,
    27,
    PORT_PCR_MUX(2),
    PORT_PCR_MUX(2),
    PORT_PCR_MUX(2),
    10,
    2,
    9,
    6,
    20,
    23,
    21,
    22,
    15,
    26,
    45,
    PORT_PCR_MUX(2),
    PORT_PCR_MUX(2),
    PORT_PCR_MUX(2),
    PORT_PCR_MUX(2),
    PORT_PCR_MUX(2),
    PORT_PCR_MUX(2),
    PORT_PCR_MUX(2),
    PORT_PCR_MUX(2),
    PORT_PCR_MUX(2),
    PORT_PCR_MUX(2),
    PORT_PCR_MUX(3),
    0x1,
    0x1,
    0x2,
    0x2,
    0x4,
    0x4,
    0x8,
    0x8,
    0x10,
    0x1,
    0x20
};

const SPIClass::SPI_Hardware_t SPIClass::spi1_hardware = {
    SIM_SCGC6,
    SIM_SCGC6_SPI1,
    1,
    IRQ_SPI1,
    32767,
    DMAMUX_SOURCE_SPI1_TX,
    DMAMUX_SOURCE_SPI1_RX,
    _spi_dma_rxISR1,
    1,
    5,
    61,
    59,
    PORT_PCR_MUX(2),
    PORT_PCR_MUX(7),
    PORT_PCR_MUX(2),
    PORT_PCR_MUX(7),
    0,
    21,
    61,
    59,
    PORT_PCR_MUX(2),
    PORT_PCR_MUX(7),
    PORT_PCR_MUX(7),
    PORT_PCR_MUX(2),
    32,
    20,
    60,
    PORT_PCR_MUX(2),
    PORT_PCR_MUX(7),
    PORT_PCR_MUX(2),
    6,
    31,
    58,
    62,
    63,
    255,
    255,
    255,
    255,
    255,
    255,
    PORT_PCR_MUX(7),
    PORT_PCR_MUX(2),
    PORT_PCR_MUX(2),
    PORT_PCR_MUX(2),
    PORT_PCR_MUX(2),
    0,
    0,
    0,
    0,
    0,
    0,
    0x1,
    0x1,
    0x2,
    0x1,
    0x4,
    0,
    0,
    0,
    0,
    0,
    0
};

void SPIClass::begin()
{
    volatile uint32_t *reg;

    hardware().clock_gate_register |= hardware().clock_gate_mask;
    port().MCR = SPI_MCR_MDIS | SPI_MCR_HALT | SPI_MCR_PCSIS(0x1F);
    port().CTAR0 =
        SPI_CTAR_FMSZ(7) | SPI_CTAR_PBR(0) | SPI_CTAR_BR(1) | SPI_CTAR_CSSCK(1);
    port().CTAR1 = SPI_CTAR_FMSZ(15) | SPI_CTAR_PBR(0) | SPI_CTAR_BR(1)
                   | SPI_CTAR_CSSCK(1);
    port().MCR = SPI_MCR_MSTR | SPI_MCR_PCSIS(0x1F);
    reg = portConfigRegister(hardware().mosi_pin[mosi_pin_index]);
    *reg = hardware().mosi_mux[mosi_pin_index];
    reg = portConfigRegister(hardware().miso_pin[miso_pin_index]);
    *reg = hardware().miso_mux[miso_pin_index];
    reg = portConfigRegister(hardware().sck_pin[sck_pin_index]);
    *reg = hardware().sck_mux[sck_pin_index];
}

void SPIClass::end()
{
    volatile uint32_t *reg;

    reg = portConfigRegister(hardware().mosi_pin[mosi_pin_index]);
    *reg = 0;
    reg = portConfigRegister(hardware().miso_pin[miso_pin_index]);
    *reg = 0;
    reg = portConfigRegister(hardware().sck_pin[sck_pin_index]);
    *reg = 0;
    port().MCR = SPI_MCR_MDIS | SPI_MCR_HALT | SPI_MCR_PCSIS(0x1F);
}

const uint16_t SPISettings::ctar_div_table[23] = { 2,   3,   4,   5,   6,   8,
                                                   10,  12,  16,  20,  24,  32,
                                                   40,  56,  64,  96,  128, 192,
                                                   256, 384, 512, 640, 768 };

const uint32_t SPISettings::ctar_clock_table[23] = {
    SPI_CTAR_PBR(0) | SPI_CTAR_BR(0) | SPI_CTAR_DBR | SPI_CTAR_CSSCK(0),
    SPI_CTAR_PBR(1) | SPI_CTAR_BR(0) | SPI_CTAR_DBR | SPI_CTAR_CSSCK(0),
    SPI_CTAR_PBR(0) | SPI_CTAR_BR(0) | SPI_CTAR_CSSCK(0),
    SPI_CTAR_PBR(2) | SPI_CTAR_BR(0) | SPI_CTAR_DBR | SPI_CTAR_CSSCK(0),
    SPI_CTAR_PBR(1) | SPI_CTAR_BR(0) | SPI_CTAR_CSSCK(0),
    SPI_CTAR_PBR(0) | SPI_CTAR_BR(1) | SPI_CTAR_CSSCK(1),
    SPI_CTAR_PBR(2) | SPI_CTAR_BR(0) | SPI_CTAR_CSSCK(0),
    SPI_CTAR_PBR(1) | SPI_CTAR_BR(1) | SPI_CTAR_CSSCK(1),
    SPI_CTAR_PBR(0) | SPI_CTAR_BR(3) | SPI_CTAR_CSSCK(2),
    SPI_CTAR_PBR(2) | SPI_CTAR_BR(1) | SPI_CTAR_CSSCK(0),
    SPI_CTAR_PBR(1) | SPI_CTAR_BR(3) | SPI_CTAR_CSSCK(2),
    SPI_CTAR_PBR(0) | SPI_CTAR_BR(4) | SPI_CTAR_CSSCK(3),
    SPI_CTAR_PBR(2) | SPI_CTAR_BR(3) | SPI_CTAR_CSSCK(2),
    SPI_CTAR_PBR(3) | SPI_CTAR_BR(3) | SPI_CTAR_CSSCK(2),
    SPI_CTAR_PBR(0) | SPI_CTAR_BR(5) | SPI_CTAR_CSSCK(4),
    SPI_CTAR_PBR(1) | SPI_CTAR_BR(5) | SPI_CTAR_CSSCK(4),
    SPI_CTAR_PBR(0) | SPI_CTAR_BR(6) | SPI_CTAR_CSSCK(5),
    SPI_CTAR_PBR(1) | SPI_CTAR_BR(6) | SPI_CTAR_CSSCK(5),
    SPI_CTAR_PBR(0) | SPI_CTAR_BR(7) | SPI_CTAR_CSSCK(6),
    SPI_CTAR_PBR(1) | SPI_CTAR_BR(7) | SPI_CTAR_CSSCK(6),
    SPI_CTAR_PBR(0) | SPI_CTAR_BR(8) | SPI_CTAR_CSSCK(7),
    SPI_CTAR_PBR(2) | SPI_CTAR_BR(7) | SPI_CTAR_CSSCK(6),
    SPI_CTAR_PBR(1) | SPI_CTAR_BR(8) | SPI_CTAR_CSSCK(7)
};

void SPIClass::updateCTAR(uint32_t ctar)
{
    if (port().CTAR0 != ctar) {
        uint32_t mcr = port().MCR;
        if (mcr & SPI_MCR_MDIS) {
            port().CTAR0 = ctar;
            port().CTAR1 = ctar | SPI_CTAR_FMSZ(8);
        } else {
            port().MCR = SPI_MCR_MDIS | SPI_MCR_HALT | SPI_MCR_PCSIS(0x1F);
            port().CTAR0 = ctar;
            port().CTAR1 = ctar | SPI_CTAR_FMSZ(8);
            port().MCR = mcr;
        }
    }
}

void SPIClass::setBitOrder(uint8_t bitOrder)
{
    hardware().clock_gate_register |= hardware().clock_gate_mask;
    uint32_t ctar = port().CTAR0;
    if (bitOrder == LSBFIRST) {
        ctar |= SPI_CTAR_LSBFE;
    } else {
        ctar &= ~SPI_CTAR_LSBFE;
    }
    updateCTAR(ctar);
}

void SPIClass::setDataMode(uint8_t dataMode)
{
    hardware().clock_gate_register |= hardware().clock_gate_mask;
}

void SPIClass::setClockDivider_noInline(uint32_t clk)
{
    hardware().clock_gate_register |= hardware().clock_gate_mask;
    uint32_t ctar = port().CTAR0;
    ctar &= (SPI_CTAR_CPOL | SPI_CTAR_CPHA | SPI_CTAR_LSBFE);
    if (ctar & SPI_CTAR_CPHA) {
        clk = (clk & 0xFFFF0FFF) | ((clk & 0xF000) >> 4);
    }
    ctar |= clk;
    updateCTAR(ctar);
}

uint8_t SPIClass::pinIsChipSelect(uint8_t pin)
{
    for (unsigned int i = 0; i < sizeof(hardware().cs_pin); i++) {
        if (pin == hardware().cs_pin[i])
            return hardware().cs_mask[i];
    }
    return 0;
}

bool SPIClass::pinIsChipSelect(uint8_t pin1, uint8_t pin2)
{
    uint8_t pin1_mask, pin2_mask;
    if ((pin1_mask = (uint8_t)pinIsChipSelect(pin1)) == 0)
        return false;
    if ((pin2_mask = (uint8_t)pinIsChipSelect(pin2)) == 0)
        return false;
    //Serial.printf("pinIsChipSelect %d %d %x %x\n\r", pin1, pin2, pin1_mask, pin2_mask);
    if ((pin1_mask & pin2_mask) != 0)
        return false;
    return true;
}

bool SPIClass::pinIsMOSI(uint8_t pin)
{
    for (unsigned int i = 0; i < sizeof(hardware().mosi_pin); i++) {
        if (pin == hardware().mosi_pin[i])
            return true;
    }
    return false;
}

bool SPIClass::pinIsMISO(uint8_t pin)
{
    for (unsigned int i = 0; i < sizeof(hardware().miso_pin); i++) {
        if (pin == hardware().miso_pin[i])
            return true;
    }
    return false;
}

bool SPIClass::pinIsSCK(uint8_t pin)
{
    for (unsigned int i = 0; i < sizeof(hardware().sck_pin); i++) {
        if (pin == hardware().sck_pin[i])
            return true;
    }
    return false;
}

// setCS() is not intended for use from normal Arduino programs/sketches.
uint8_t SPIClass::setCS(uint8_t pin)
{
    for (unsigned int i = 0; i < sizeof(hardware().cs_pin); i++) {
        if (pin == hardware().cs_pin[i]) {
            volatile uint32_t *reg = portConfigRegister(pin);
            *reg = hardware().cs_mux[i];
            return hardware().cs_mask[i];
        }
    }
    return 0;
}

void SPIClass::setMOSI(uint8_t pin)
{
    if (pin != hardware().mosi_pin[mosi_pin_index]) {
        for (unsigned int i = 0; i < sizeof(hardware().mosi_pin); i++) {
            if (pin == hardware().mosi_pin[i]) {
                if (hardware().clock_gate_register
                    & hardware().clock_gate_mask) {
                    volatile uint32_t *reg;
                    reg =
                        portConfigRegister(hardware().mosi_pin[mosi_pin_index]);
                    *reg = 0;
                    reg = portConfigRegister(hardware().mosi_pin[i]);
                    *reg = hardware().mosi_mux[i];
                }
                mosi_pin_index = i;
                return;
            }
        }
    }
}

void SPIClass::setMISO(uint8_t pin)
{
    if (pin != hardware().miso_pin[miso_pin_index]) {
        for (unsigned int i = 0; i < sizeof(hardware().miso_pin); i++) {
            if (pin == hardware().miso_pin[i]) {
                if (hardware().clock_gate_register
                    & hardware().clock_gate_mask) {
                    volatile uint32_t *reg;
                    reg =
                        portConfigRegister(hardware().miso_pin[miso_pin_index]);
                    *reg = 0;
                    reg = portConfigRegister(hardware().miso_pin[i]);
                    *reg = hardware().miso_mux[i];
                }
                miso_pin_index = i;
                return;
            }
        }
    }
}

void SPIClass::setSCK(uint8_t pin)
{
    if (pin != hardware().sck_pin[sck_pin_index]) {
        for (unsigned int i = 0; i < sizeof(hardware().sck_pin); i++) {
            if (pin == hardware().sck_pin[i]) {
                if (hardware().clock_gate_register
                    & hardware().clock_gate_mask) {
                    volatile uint32_t *reg;
                    reg = portConfigRegister(hardware().sck_pin[sck_pin_index]);
                    *reg = 0;
                    reg = portConfigRegister(hardware().sck_pin[i]);
                    *reg = hardware().sck_mux[i];
                }
                sck_pin_index = i;
                return;
            }
        }
    }
}

void SPIClass::transfer(const void *buf, void *retbuf, size_t count)
{
    if (count == 0)
        return;
    if (!(port().CTAR0 & SPI_CTAR_LSBFE)) {
        // We are doing the standard MSB order
        const uint8_t *p_write = (const uint8_t *)buf;
        uint8_t *p_read = (uint8_t *)retbuf;
        size_t count_read = count;

        // Lets clear the reader queue
        port().MCR = SPI_MCR_MSTR | SPI_MCR_CLR_RXF | SPI_MCR_PCSIS(0x1F);

        uint32_t sr;

        // Now lets loop while we still have data to output
        if (count & 1) {
            if (p_write) {
                if (count > 1)
                    port().PUSHR =
                        *p_write++ | SPI_PUSHR_CONT | SPI_PUSHR_CTAS(0);
                else
                    port().PUSHR = *p_write++ | SPI_PUSHR_CTAS(0);
            } else {
                if (count > 1)
                    port().PUSHR =
                        _transferWriteFill | SPI_PUSHR_CONT | SPI_PUSHR_CTAS(0);
                else
                    port().PUSHR = _transferWriteFill | SPI_PUSHR_CTAS(0);
            }
            count--;
        }

        uint16_t w = (uint16_t)(_transferWriteFill << 8) | _transferWriteFill;

        while (count > 0) {
            // Push out the next byte;
            if (p_write) {
                w = (*p_write++) << 8;
                w |= *p_write++;
            }
            uint16_t queue_full_status_mask = (hardware().queue_size - 1) << 12;
            if (count == 2)
                port().PUSHR = w | SPI_PUSHR_CTAS(1);
            else
                port().PUSHR = w | SPI_PUSHR_CONT | SPI_PUSHR_CTAS(1);
            count -= 2; // how many bytes to output.
            // Make sure queue is not full before pushing next byte out
            do {
                sr = port().SR;
                if (sr & 0xF0) {
                    uint16_t w = port().POPR; // Read any pending RX bytes in
                    if (count_read & 1) {
                        if (p_read) {
                            *p_read++ = w; // Read any pending RX bytes in
                        }
                        count_read--;
                    } else {
                        if (p_read) {
                            *p_read++ = w >> 8;
                            *p_read++ = (w & 0xff);
                        }
                        count_read -= 2;
                    }
                }
            } while ((sr & (15 << 12)) > queue_full_status_mask);
        }

        // now lets wait for all of the read bytes to be returned...
        while (count_read) {
            sr = port().SR;
            if (sr & 0xF0) {
                uint16_t w = port().POPR; // Read any pending RX bytes in
                if (count_read & 1) {
                    if (p_read)
                        *p_read++ = w; // Read any pending RX bytes in
                    count_read--;
                } else {
                    if (p_read) {
                        *p_read++ = w >> 8;
                        *p_read++ = (w & 0xff);
                    }
                    count_read -= 2;
                }
            }
        }
    } else {
        // We are doing the less ofen LSB mode
        const uint8_t *p_write = (const uint8_t *)buf;
        uint8_t *p_read = (uint8_t *)retbuf;
        size_t count_read = count;

        // Lets clear the reader queue
        port().MCR = SPI_MCR_MSTR | SPI_MCR_CLR_RXF | SPI_MCR_PCSIS(0x1F);

        uint32_t sr;

        // Now lets loop while we still have data to output
        if (count & 1) {
            if (p_write) {
                if (count > 1)
                    port().PUSHR =
                        *p_write++ | SPI_PUSHR_CONT | SPI_PUSHR_CTAS(0);
                else
                    port().PUSHR = *p_write++ | SPI_PUSHR_CTAS(0);
            } else {
                if (count > 1)
                    port().PUSHR =
                        _transferWriteFill | SPI_PUSHR_CONT | SPI_PUSHR_CTAS(0);
                else
                    port().PUSHR = _transferWriteFill | SPI_PUSHR_CTAS(0);
            }
            count--;
        }

        uint16_t w = _transferWriteFill;

        while (count > 0) {
            // Push out the next byte;
            if (p_write) {
                w = *p_write++;
                w |= ((*p_write++) << 8);
            }
            uint16_t queue_full_status_mask = (hardware().queue_size - 1) << 12;
            if (count == 2)
                port().PUSHR = w | SPI_PUSHR_CTAS(1);
            else
                port().PUSHR = w | SPI_PUSHR_CONT | SPI_PUSHR_CTAS(1);
            count -= 2; // how many bytes to output.
            // Make sure queue is not full before pushing next byte out
            do {
                sr = port().SR;
                if (sr & 0xF0) {
                    uint16_t w = port().POPR; // Read any pending RX bytes in
                    if (count_read & 1) {
                        if (p_read) {
                            *p_read++ = w; // Read any pending RX bytes in
                        }
                        count_read--;
                    } else {
                        if (p_read) {
                            *p_read++ = (w & 0xff);
                            *p_read++ = w >> 8;
                        }
                        count_read -= 2;
                    }
                }
            } while ((sr & (15 << 12)) > queue_full_status_mask);
        }

        // now lets wait for all of the read bytes to be returned...
        while (count_read) {
            sr = port().SR;
            if (sr & 0xF0) {
                uint16_t w = port().POPR; // Read any pending RX bytes in
                if (count_read & 1) {
                    if (p_read)
                        *p_read++ = w; // Read any pending RX bytes in
                    count_read--;
                } else {
                    if (p_read) {
                        *p_read++ = (w & 0xff);
                        *p_read++ = w >> 8;
                    }
                    count_read -= 2;
                }
            }
        }
    }
}

SPIClass SPI((uintptr_t)&KINETISK_SPI0, (uintptr_t)&SPIClass::spi0_hardware);
SPIClass SPI1((uintptr_t)&KINETISK_SPI1, (uintptr_t)&SPIClass::spi1_hardware);