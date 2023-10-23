#include "usb_dev.h"

#include "kinetis.h"
#include "usb_mem.h"

__attribute__ ((section(".usbbuffers"), used))
unsigned char usb_buffer_memory[NUM_USB_BUFFERS * sizeof(usb_packet_t)];

static uint32_t usb_buffer_available = 0xFFFFFFFF;

// use bitmask and CLZ instruction to implement fast free list
// http://www.archivum.info/gnu.gcc.help/2006-08/00148/Re-GCC-Inline-Assembly.html
// http://gcc.gnu.org/ml/gcc/2012-06/msg00015.html
// __builtin_clz()

usb_packet_t * usb_malloc(void)
{
	unsigned int n, avail;
	uint8_t *p;

	__disable_irq();
	avail = usb_buffer_available;
	n = __builtin_clz(avail); // clz = count leading zeros
	if (n >= NUM_USB_BUFFERS) {
		__enable_irq();
		return NULL;
	}

	usb_buffer_available = avail & ~(0x80000000 >> n);
	__enable_irq();
	p = usb_buffer_memory + (n * sizeof(usb_packet_t));

	*(uint32_t *)p = 0;
	*(uint32_t *)(p + 4) = 0;
	return (usb_packet_t *)p;
}

// for the receive endpoints to request memory
extern uint8_t usb_rx_memory_needed;
extern void usb_rx_memory(usb_packet_t *packet);

void usb_free(usb_packet_t *p)
{
	unsigned int n, mask;

	n = ((uint8_t *)p - usb_buffer_memory) / sizeof(usb_packet_t);
	if (n >= NUM_USB_BUFFERS) return;

	// if any endpoints are starving for memory to receive
	// packets, give this memory to them immediately!
	if (usb_rx_memory_needed && usb_configuration) {
		usb_rx_memory(p);
		return;
	}

	mask = (0x80000000 >> n);
	__disable_irq();
	usb_buffer_available |= mask;
	__enable_irq();
}
