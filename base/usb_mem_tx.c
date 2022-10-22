#include "usb_dev.h"

#include "kinetis.h"
//#include "HardwareSerial.h"
#include "usb_mem_tx.h"

__attribute__ ((section(".usbbuffers"), used))
unsigned char usb_buffer_memory_tx[NUM_USB_BUFFERS * sizeof(usb_packet_t)];

static uint32_t usb_buffer_available_tx = 0xFFFFFFFF;

// use bitmask and CLZ instruction to implement fast free list
// http://www.archivum.info/gnu.gcc.help/2006-08/00148/Re-GCC-Inline-Assembly.html
// http://gcc.gnu.org/ml/gcc/2012-06/msg00015.html
// __builtin_clz()

usb_packet_t * usb_malloc_tx(void)
{
	unsigned int n, avail;
	uint8_t *p;

	__disable_irq();
	avail = usb_buffer_available_tx;
	n = __builtin_clz(avail); // clz = count leading zeros
	if (n >= NUM_USB_BUFFERS) {
		__enable_irq();
		return NULL;
	}
	//serial_print("malloc:");
	//serial_phex(n);
	//serial_print("\n");

	usb_buffer_available_tx = avail & ~(0x80000000 >> n);
	__enable_irq();
	p = usb_buffer_memory_tx + (n * sizeof(usb_packet_t));
	//serial_print("malloc:");
	//serial_phex32((int)p);
	//serial_print("\n");
	*(uint32_t *)p = 0;
	*(uint32_t *)(p + 4) = 0;
	return (usb_packet_t *)p;
}

void usb_free_tx(usb_packet_t *p)
{
	unsigned int n, mask;

	//serial_print("free:");
	n = ((uint8_t *)p - usb_buffer_memory_tx) / sizeof(usb_packet_t);
	if (n >= NUM_USB_BUFFERS) return;
	//serial_phex(n);
	//serial_print("\n");

	mask = (0x80000000 >> n);
	__disable_irq();
	usb_buffer_available_tx |= mask;
	__enable_irq();

	//serial_print("free:");
	//serial_phex32((int)p);
	//serial_print("\n");
}
