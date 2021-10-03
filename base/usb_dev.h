#ifndef _usb_dev_h_
#define _usb_dev_h_

#include "usb_desc.h"
#include "usb_mem.h"

#ifdef __cplusplus
extern "C" {
#endif

extern volatile int rxIsrDisabled;

void usb_init (void);
void usb_init_serialnumber (void);
void usb_isr (void);
usb_packet_t *usb_rx (uint32_t endpoint);
uint32_t usb_tx_byte_count (uint32_t endpoint);
uint32_t usb_tx_packet_count (uint32_t endpoint);
void usb_tx (uint32_t endpoint, usb_packet_t *packet);

extern volatile uint8_t usb_configuration;
extern uint16_t usb_rx_byte_count_data;


#ifdef MIDI_INTERFACE
extern void usb_midi_flush_output (void);
#endif

#ifdef __cplusplus
}
#endif

#else // F_CPU < 20000000

#ifdef __cplusplus
extern "C" {
#endif

void usb_init (void);

#ifdef __cplusplus
}
#endif

#endif
