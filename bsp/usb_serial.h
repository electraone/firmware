#ifndef USBserial_h_
#define USBserial_h_

#include "usb_desc.h"
#include <inttypes.h>

// Allow Arduino programs using Serial to compile, but Serial will do nothing.
#ifdef __cplusplus
#include "Stream.h"
class usb_serial_class : public Stream
{
public:
	constexpr usb_serial_class() {}
        void begin(long) { };
        void end() { };
        virtual int available() { return 0; }
        virtual int read() { return -1; }
        virtual int peek() { return -1; }
        virtual void flush() { }
        virtual void clear() { }
        virtual size_t write(uint8_t c) { return 1; }
        virtual size_t write(const uint8_t *buffer, size_t size) { return size; }
	size_t write(unsigned long n) { return 1; }
	size_t write(long n) { return 1; }
	size_t write(unsigned int n) { return 1; }
	size_t write(int n) { return 1; }
	int availableForWrite() { return 0; }
	using Print::write;
        void send_now(void) { }
        uint32_t baud(void) { return 0; }
        uint8_t stopbits(void) { return 1; }
        uint8_t paritytype(void) { return 0; }
        uint8_t numbits(void) { return 8; }
        uint8_t dtr(void) { return 1; }
        uint8_t rts(void) { return 1; }
        operator bool() { return true; }
};

extern usb_serial_class Serial;
extern void serialEvent(void);
#endif // __cplusplus

#endif // USBserial_h_
