#include "Uart.h"

Uart Serial1(1);
Uart Serial2(2);

#define BAUD2DIV(baud) (((F_CPU * 2) + ((baud) >> 1)) / (baud))
#define IRQ_PRIORITY 2 // 0 = highest priority, 255 = lowest

#define C2_ENABLE UART_C2_TE | UART_C2_RE | UART_C2_RIE | UART_C2_ILIE
#define C2_TX_ACTIVE C2_ENABLE | UART_C2_TIE
#define C2_TX_COMPLETING C2_ENABLE | UART_C2_TCIE
#define C2_TX_INACTIVE C2_ENABLE

Uart::Uart(uint8_t newSerialId) : serialId(newSerialId)
{
}

void Uart::begin(uint32_t baud)
{
    uint32_t divisor = BAUD2DIV(baud);
    rx_buffer_head = 0;
    rx_buffer_tail = 0;
    tx_buffer_head = 0;
    tx_buffer_tail = 0;
    transmitting = 0;

    if (divisor < 32) {
        divisor = 32;
    }

    if (serialId == 1) {
        SIM_SCGC4 |= SIM_SCGC4_UART0;
        UART0_BDH = (divisor >> 13) & 0x1F;
        UART0_BDL = (divisor >> 5) & 0xFF;
        UART0_C4 = divisor & 0x1F;
        UART0_C1 = UART_C1_ILT;
        UART0_TWFIFO = 2; // tx watermark, causes S1_TDRE to set
        UART0_RWFIFO = 4; // rx watermark, causes S1_RDRF to set
        UART0_PFIFO = UART_PFIFO_TXFE | UART_PFIFO_RXFE;

        UART0_C2 = C2_TX_INACTIVE;
        NVIC_SET_PRIORITY(IRQ_UART0_STATUS, IRQ_PRIORITY);
        NVIC_ENABLE_IRQ(IRQ_UART0_STATUS);
    } else {
        SIM_SCGC4 |= SIM_SCGC4_UART1;
        UART1_BDH = (divisor >> 13) & 0x1F;
        UART1_BDL = (divisor >> 5) & 0xFF;
        UART1_C4 = divisor & 0x1F;
        UART1_C1 = UART_C1_ILT;
        UART1_TWFIFO = 2; // tx watermark, causes S1_TDRE to set
        UART1_RWFIFO = 4; // rx watermark, causes S1_RDRF to set
        UART1_PFIFO = UART_PFIFO_TXFE | UART_PFIFO_RXFE;
        UART1_C2 = C2_TX_INACTIVE;
        NVIC_SET_PRIORITY(IRQ_UART1_STATUS, IRQ_PRIORITY);
        NVIC_ENABLE_IRQ(IRQ_UART1_STATUS);
    }
}

int Uart::available(void)
{
    uint32_t head, tail;

    head = rx_buffer_head;
    tail = rx_buffer_tail;

    if (head >= tail)
        return head - tail;
    return SERIAL_RX_BUFFER_SIZE + head - tail;
}

int Uart::peek(void)
{
    uint32_t head, tail;

    head = rx_buffer_head;
    tail = rx_buffer_tail;
    if (head == tail)
        return -1;
    if (++tail >= SERIAL_RX_BUFFER_SIZE)
        tail = 0;
    return rx_buffer[tail];
}

int Uart::read(void)
{
    uint32_t head, tail;
    int c;

    head = rx_buffer_head;
    tail = rx_buffer_tail;
    if (head == tail)
        return -1;
    if (++tail >= SERIAL_RX_BUFFER_SIZE)
        tail = 0;
    c = rx_buffer[tail];
    rx_buffer_tail = tail;
    return c;
}

void Uart::flush(void)
{
    while (transmitting) {}
}

void Uart::clear(void)
{
    if (serialId == 1) {
        if (!(SIM_SCGC4 & SIM_SCGC4_UART0))
            return;
        UART0_C2 &= ~(UART_C2_RE | UART_C2_RIE | UART_C2_ILIE);
        UART0_CFIFO = UART_CFIFO_RXFLUSH;
        UART0_C2 |= (UART_C2_RE | UART_C2_RIE | UART_C2_ILIE);
    } else {
        if (!(SIM_SCGC4 & SIM_SCGC4_UART1))
            return;
        UART1_C2 &= ~(UART_C2_RE | UART_C2_RIE | UART_C2_ILIE);
        UART1_CFIFO = UART_CFIFO_RXFLUSH;
        UART1_C2 |= (UART_C2_RE | UART_C2_RIE | UART_C2_ILIE);
    }

    rx_buffer_head = rx_buffer_tail;
}

int Uart::availableForWrite(void)
{
    uint32_t head, tail;

    head = tx_buffer_head;
    tail = tx_buffer_tail;
    if (head >= tail)
        return SERIAL_TX_BUFFER_SIZE - 1 - head + tail;
    return tail - head - 1;
}

size_t Uart::write(uint8_t c)
{
    uint32_t head, n;

    if ((serialId == 1 && !(SIM_SCGC4 & SIM_SCGC4_UART0))
        || (serialId == 2 && !(SIM_SCGC4 & SIM_SCGC4_UART1))) {
        return 0;
    }

    head = tx_buffer_head;
    if (++head >= SERIAL_TX_BUFFER_SIZE)
        head = 0;
    while (tx_buffer_tail == head) {
        int priority = nvic_execution_priority();
        if (priority <= IRQ_PRIORITY) {
            if (((UART0_S1 || UART1_S1) & UART_S1_TDRE)) {
                uint32_t tail = tx_buffer_tail;
                if (++tail >= SERIAL_TX_BUFFER_SIZE)
                    tail = 0;
                n = tx_buffer[tail];
                if (serialId == 1) {
                    UART0_D = n;
                } else {
                    UART1_D = n;
                }
                tx_buffer_tail = tail;
            }
        }
    }
    tx_buffer[head] = c;
    transmitting = 1;
    tx_buffer_head = head;

    if (serialId == 1) {
        UART0_C2 = C2_TX_ACTIVE;
    } else {
        UART1_C2 = C2_TX_ACTIVE;
    }

    return 1;
}

void uart0_status_isr(void)
{
    uint32_t head, tail, n;
    uint8_t c;
    uint32_t newhead;
    uint8_t avail;

    if (UART0_S1 & (UART_S1_RDRF | UART_S1_IDLE)) {
        __disable_irq();
        avail = UART0_RCFIFO;
        if (avail == 0) {
            c = UART0_D;
            UART0_CFIFO = UART_CFIFO_RXFLUSH;
            __enable_irq();
        } else {
            __enable_irq();
            head = Serial1.rx_buffer_head;
            tail = Serial1.rx_buffer_tail;
            do {
                n = UART0_D;
                newhead = head + 1;
                if (newhead >= SERIAL_RX_BUFFER_SIZE)
                    newhead = 0;
                if (newhead != tail) {
                    head = newhead;
                    Serial1.rx_buffer[head] = n;
                }
            } while (--avail > 0);
            Serial1.rx_buffer_head = head;
        }
    }
    c = UART0_C2;
    if ((c & UART_C2_TIE) && (UART0_S1 & UART_S1_TDRE)) {
        head = Serial1.tx_buffer_head;
        tail = Serial1.tx_buffer_tail;
        do {
            if (tail == head)
                break;
            if (++tail >= SERIAL_TX_BUFFER_SIZE)
                tail = 0;
            avail = UART0_S1;
            n = Serial1.tx_buffer[tail];
            UART0_D = n;
        } while (UART0_TCFIFO < 8);
        Serial1.tx_buffer_tail = tail;
        if (UART0_S1 & UART_S1_TDRE)
            UART0_C2 = C2_TX_COMPLETING;
    }

    if ((c & UART_C2_TCIE) && (UART0_S1 & UART_S1_TC)) {
        Serial1.transmitting = 0;
        UART0_C2 = C2_TX_INACTIVE;
    }
}

void uart1_status_isr(void)
{
    uint32_t head, tail, n;
    uint8_t c;
    uint32_t newhead;
    uint8_t avail;

    if (UART1_S1 & (UART_S1_RDRF | UART_S1_IDLE)) {
        __disable_irq();
        avail = UART1_RCFIFO;
        if (avail == 0) {
            c = UART1_D;
            UART1_CFIFO = UART_CFIFO_RXFLUSH;
            __enable_irq();
        } else {
            __enable_irq();
            head = Serial2.rx_buffer_head;
            tail = Serial2.rx_buffer_tail;
            do {
                n = UART1_D;
                newhead = head + 1;
                if (newhead >= SERIAL_RX_BUFFER_SIZE)
                    newhead = 0;
                if (newhead != tail) {
                    head = newhead;
                    Serial2.rx_buffer[head] = n;
                }
            } while (--avail > 0);
            Serial2.rx_buffer_head = head;
        }
    }
    c = UART1_C2;
    if ((c & UART_C2_TIE) && (UART1_S1 & UART_S1_TDRE)) {
        head = Serial2.tx_buffer_head;
        tail = Serial2.tx_buffer_tail;
        do {
            if (tail == head)
                break;
            if (++tail >= SERIAL_TX_BUFFER_SIZE)
                tail = 0;
            avail = UART1_S1;
            n = Serial2.tx_buffer[tail];
            UART1_D = n;
        } while (UART1_TCFIFO < 8);
        Serial2.tx_buffer_tail = tail;
        if (UART1_S1 & UART_S1_TDRE)
            UART1_C2 = C2_TX_COMPLETING;
    }

    if ((c & UART_C2_TCIE) && (UART1_S1 & UART_S1_TC)) {
        Serial2.transmitting = 0;
        UART1_C2 = C2_TX_INACTIVE;
    }
}
