#ifndef _usb_mem_tx_h_
#define _usb_mem_tx_h_

#include <stdint.h>
#include "usb_mem.h"

#ifdef __cplusplus
extern "C" {
#endif

usb_packet_t * usb_malloc_tx(void);
void usb_free_tx(usb_packet_t *p);

#ifdef __cplusplus
}
#endif


#endif
