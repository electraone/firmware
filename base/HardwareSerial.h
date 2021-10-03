#ifndef HardwareSerial_h
#define HardwareSerial_h

#include "kinetis.h"

// Uncomment to enable 9 bit formats.  These are default disabled to save memory.
//#define SERIAL_9BIT_SUPPORT
//
// On Windows & Linux, this file is in Arduino's hardware/teensy/avr/cores/teensy3
//   folder.  The Windows installer puts Arduino in C:\Program Files (x86)\Arduino
// On Macintosh, you must control-click Arduino and select "Show Package Contents", then
//   look in Contents/Java/hardware/teensy/avr/cores/teensy3 to find this file.
//
// Teensy 3.x boards support 9 bit mode on all their serial ports
// Teensy LC only supports 9 bit mode on Serial1.  Serial2 & Serial3 can't use 9 bits.


#define SERIAL_7E1 0x02
#define SERIAL_7O1 0x03
#define SERIAL_8N1 0x00
#define SERIAL_8E1 0x06
#define SERIAL_8O1 0x07
#define SERIAL_7E1_RXINV 0x12
#define SERIAL_7O1_RXINV 0x13
#define SERIAL_8N1_RXINV 0x10
#define SERIAL_8E1_RXINV 0x16
#define SERIAL_8O1_RXINV 0x17
#define SERIAL_7E1_TXINV 0x22
#define SERIAL_7O1_TXINV 0x23
#define SERIAL_8N1_TXINV 0x20
#define SERIAL_8E1_TXINV 0x26
#define SERIAL_8O1_TXINV 0x27
#define SERIAL_7E1_RXINV_TXINV 0x32
#define SERIAL_7O1_RXINV_TXINV 0x33
#define SERIAL_8N1_RXINV_TXINV 0x30
#define SERIAL_8E1_RXINV_TXINV 0x36
#define SERIAL_8O1_RXINV_TXINV 0x37
#ifdef SERIAL_9BIT_SUPPORT
#define SERIAL_9N1 0x84
#define SERIAL_9E1 0x8E
#define SERIAL_9O1 0x8F
#define SERIAL_9N1_RXINV 0x94
#define SERIAL_9E1_RXINV 0x9E
#define SERIAL_9O1_RXINV 0x9F
#define SERIAL_9N1_TXINV 0xA4
#define SERIAL_9E1_TXINV 0xAE
#define SERIAL_9O1_TXINV 0xAF
#define SERIAL_9N1_RXINV_TXINV 0xB4
#define SERIAL_9E1_RXINV_TXINV 0xBE
#define SERIAL_9O1_RXINV_TXINV 0xBF
#endif

#define SERIAL_2STOP_BITS 0x100
#define SERIAL_8E2 (SERIAL_8E1 | SERIAL_2STOP_BITS)
#define SERIAL_8O2 (SERIAL_8O1 | SERIAL_2STOP_BITS)
#define SERIAL_8E2_RXINV (SERIAL_8E1_RXINV | SERIAL_2STOP_BITS)
#define SERIAL_8O2_RXINV (SERIAL_8O1_RXINV | SERIAL_2STOP_BITS)
#define SERIAL_8E2_TXINV (SERIAL_8E1_TXINV | SERIAL_2STOP_BITS)
#define SERIAL_8O2_TXINV (SERIAL_8O1_TXINV | SERIAL_2STOP_BITS)
#define SERIAL_8E2_RXINV_TXINV (SERIAL_8E1_RXINV_TXINV | SERIAL_2STOP_BITS)
#define SERIAL_8O2_RXINV_TXINV (SERIAL_8O1_RXINV_TXINV | SERIAL_2STOP_BITS)
#define SERIAL_8N2 (SERIAL_8N1 | SERIAL_2STOP_BITS)
#define SERIAL_8N2_RXINV (SERIAL_8N1_RXINV | SERIAL_2STOP_BITS)
#define SERIAL_8N2_TXINV (SERIAL_8N1_TXINV | SERIAL_2STOP_BITS)
#define SERIAL_8N2_RXINV_TXINV (SERIAL_8N1_RXINV_TXINV | SERIAL_2STOP_BITS)

// bit0: parity, 0=even, 1=odd
// bit1: parity, 0=disable, 1=enable
// bit2: mode, 1=9bit, 0=8bit
// bit3: mode10: 1=10bit, 0=8bit
// bit4: rxinv, 0=normal, 1=inverted
// bit5: txinv, 0=normal, 1=inverted
// bit6: unused
// bit7: actual data goes into 9th bit


#if defined(KINETISK)
#define BAUD2DIV(baud)  (((F_CPU * 2) + ((baud) >> 1)) / (baud))
#define BAUD2DIV2(baud) (((F_CPU * 2) + ((baud) >> 1)) / (baud))
#define BAUD2DIV3(baud) (((F_BUS * 2) + ((baud) >> 1)) / (baud))
#elif defined(KINETISL)

#if F_CPU <= 2000000
#define BAUD2DIV(baud)  (((F_PLL / 16 ) + ((baud) >> 1)) / (baud))
#elif F_CPU <= 16000000
#define BAUD2DIV(baud)  (((F_PLL / (F_PLL / 1000000)) + ((baud) >> 1)) / (baud))
#else
#define BAUD2DIV(baud)  (((F_PLL / 2 / 16) + ((baud) >> 1)) / (baud))
#endif

#define BAUD2DIV2(baud) (((F_BUS / 16) + ((baud) >> 1)) / (baud))
#define BAUD2DIV3(baud) (((F_BUS / 16) + ((baud) >> 1)) / (baud))
#endif


// C language implementation
//
#ifdef __cplusplus
extern "C" {
#endif
void serial_begin(uint32_t divisor);
void serial_format(uint32_t format);
void serial_end(void);
void serial_set_transmit_pin(uint8_t pin);
void serial_set_rx(uint8_t pin);
void serial_set_tx(uint8_t pin, uint8_t opendrain);
int serial_set_rts(uint8_t pin);
int serial_set_cts(uint8_t pin);
void serial_putchar(uint32_t c);
void serial_write(const void *buf, unsigned int count);
void serial_flush(void);
int serial_write_buffer_free(void);
int serial_available(void);
int serial_getchar(void);
int serial_peek(void);
void serial_clear(void);
void serial_print(const char *p);
void serial_phex(uint32_t n);
void serial_phex16(uint32_t n);
void serial_phex32(uint32_t n);

void serial2_begin(uint32_t divisor);
void serial2_format(uint32_t format);
void serial2_end(void);
void serial2_set_transmit_pin(uint8_t pin);
void serial2_set_rx(uint8_t pin);
void serial2_set_tx(uint8_t pin, uint8_t opendrain);
int serial2_set_rts(uint8_t pin);
int serial2_set_cts(uint8_t pin);
void serial2_putchar(uint32_t c);
void serial2_write(const void *buf, unsigned int count);
void serial2_flush(void);
int serial2_write_buffer_free(void);
int serial2_available(void);
int serial2_getchar(void);
int serial2_peek(void);
void serial2_clear(void);

#ifdef __cplusplus
}
#endif


// C++ interface
//
#ifdef __cplusplus
#include "Stream.h"
class HardwareSerial : public Stream
{
public:
	constexpr HardwareSerial() {}
	virtual void begin(uint32_t baud) { serial_begin(BAUD2DIV(baud)); }
	virtual void begin(uint32_t baud, uint32_t format) {
					  serial_begin(BAUD2DIV(baud));
					  serial_format(format); }
	virtual void end(void)		{ serial_end(); }
	virtual void transmitterEnable(uint8_t pin) { serial_set_transmit_pin(pin); }
	virtual void setRX(uint8_t pin) { serial_set_rx(pin); }
	virtual void setTX(uint8_t pin, bool opendrain=false) { serial_set_tx(pin, opendrain); }
	virtual bool attachRts(uint8_t pin) { return serial_set_rts(pin); }
	virtual bool attachCts(uint8_t pin) { return serial_set_cts(pin); }
	virtual int available(void)     { return serial_available(); }
	virtual int peek(void)          { return serial_peek(); }
	virtual int read(void)          { return serial_getchar(); }
	virtual void flush(void)        { serial_flush(); }
	virtual void clear(void)	{ serial_clear(); }
	virtual int availableForWrite(void) { return serial_write_buffer_free(); }
	using Print::write;
	virtual size_t write(uint8_t c) { serial_putchar(c); return 1; }
	virtual size_t write(unsigned long n)   { return write((uint8_t)n); }
	virtual size_t write(long n)            { return write((uint8_t)n); }
	virtual size_t write(unsigned int n)    { return write((uint8_t)n); }
	virtual size_t write(int n)             { return write((uint8_t)n); }
	virtual size_t write(const uint8_t *buffer, size_t size)
					{ serial_write(buffer, size); return size; }
        virtual size_t write(const char *str)	{ size_t len = strlen(str);
					  serial_write((const uint8_t *)str, len);
					  return len; }
	virtual size_t write9bit(uint32_t c)	{ serial_putchar(c); return 1; }
	operator bool()			{ return true; }
};
extern HardwareSerial Serial1;
extern void serialEvent1(void);

class HardwareSerial2 : public HardwareSerial
{
public:
	constexpr HardwareSerial2() {}
	virtual void begin(uint32_t baud) { serial2_begin(BAUD2DIV2(baud)); }
	virtual void begin(uint32_t baud, uint32_t format) {
					  serial2_begin(BAUD2DIV2(baud));
					  serial2_format(format); }
	virtual void end(void)		{ serial2_end(); }
	virtual void transmitterEnable(uint8_t pin) { serial2_set_transmit_pin(pin); }
	virtual void setRX(uint8_t pin) { serial2_set_rx(pin); }
	virtual void setTX(uint8_t pin, bool opendrain=false) { serial2_set_tx(pin, opendrain); }
	virtual bool attachRts(uint8_t pin) { return serial2_set_rts(pin); }
	virtual bool attachCts(uint8_t pin) { return serial2_set_cts(pin); }
	virtual int available(void)     { return serial2_available(); }
	virtual int peek(void)          { return serial2_peek(); }
	virtual int read(void)          { return serial2_getchar(); }
	virtual void flush(void)        { serial2_flush(); }
	virtual void clear(void)	{ serial2_clear(); }
	virtual int availableForWrite(void) { return serial2_write_buffer_free(); }
	using Print::write;
	virtual size_t write(uint8_t c) { serial2_putchar(c); return 1; }
	virtual size_t write(unsigned long n)   { return write((uint8_t)n); }
	virtual size_t write(long n)            { return write((uint8_t)n); }
	virtual size_t write(unsigned int n)    { return write((uint8_t)n); }
	virtual size_t write(int n)             { return write((uint8_t)n); }
	virtual size_t write(const uint8_t *buffer, size_t size)
					{ serial2_write(buffer, size); return size; }
        virtual size_t write(const char *str)	{ size_t len = strlen(str);
					  serial2_write((const uint8_t *)str, len);
					  return len; }
	virtual size_t write9bit(uint32_t c)	{ serial2_putchar(c); return 1; }
	operator bool()			{ return true; }
};
extern HardwareSerial2 Serial2;
extern void serialEvent2(void);

#endif
#endif
