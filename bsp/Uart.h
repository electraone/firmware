#pragma once

#include "mk66fx1m0.h"
#include "Stream.h"

#define SERIAL_TX_BUFFER_SIZE 256
#define SERIAL_RX_BUFFER_SIZE 1024

class Uart : public Stream
{
public:
    Uart(uint8_t newSerialId);
    ~Uart() = default;

    void begin(uint32_t baud);
    int available(void);
    int peek(void);
    int read(void);
    void flush(void);
    void clear(void);
    int availableForWrite(void);
    using Print::write;
    size_t write(uint8_t c);

    volatile uint8_t tx_buffer[SERIAL_TX_BUFFER_SIZE];
    volatile uint8_t rx_buffer[SERIAL_RX_BUFFER_SIZE];
    volatile uint8_t transmitting = 0;

    volatile uint16_t tx_buffer_head = 0;
    volatile uint16_t tx_buffer_tail = 0;

    volatile uint16_t rx_buffer_head = 0;
    volatile uint16_t rx_buffer_tail = 0;
    uint8_t serialId = 0;
};

extern Uart Serial1;
extern Uart Serial2;
