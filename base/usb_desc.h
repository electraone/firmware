#ifndef _usb_desc_h_
#define _usb_desc_h_

#include <stdint.h>
#include <stddef.h>


#define MIDI_NAME            { 'E', 'l', 'e', 'c', 't', 'r', 'a', ' ', 'C', 'o', 'n', 't', 'r', 'o', 'l', 'l', 'e', 'r' }
#define MIDI_NAME_LEN  18
#define MANUFACTURER_NAME    { 'e', 'l', 'e', 'c', 't', 'r', 'a', '.', 'o', 'n', 'e' }
#define MANUFACTURER_NAME_LEN 11
#define SERIAL_NUMBER        { '1', '2', '3', '4', '1', '2', '3', '4', '5', '6' }
#define SERIAL_NUMBER_LEN 10
#define MIDI_PORT1_NAME      { 'E', 'l', 'e', 'c', 't', 'r', 'a', ' ', 'P', 'o', 'r', 't', ' ', '1' }
#define MIDI_PORT1_NAME_LEN 14
#define MIDI_PORT2_NAME      { 'E', 'l', 'e', 'c', 't', 'r', 'a', ' ', 'P', 'o', 'r', 't', ' ', '2' }
#define MIDI_PORT2_NAME_LEN 14
#define MIDI_PORT3_NAME      { 'E', 'l', 'e', 'c', 't', 'r', 'a', ' ', 'C', 'T', 'R', 'L' }
#define MIDI_PORT3_NAME_LEN 12

#define ENDPOINT_UNUSED			          0x00
#define ENDPOINT_TRANSIMIT_ONLY		    0x15
#define ENDPOINT_RECEIVE_ONLY		      0x19
#define ENDPOINT_TRANSMIT_AND_RECEIVE	0x1D
#define ENDPOINT_RECEIVE_ISOCHRONOUS	0x18
#define ENDPOINT_TRANSMIT_ISOCHRONOUS	0x14

#define VENDOR_ID		          0x1FC9
#define PRODUCT_ID		        0x82CF
#define BCD_DEVICE            0x0213
#define EP0_ENDPOINT          0
#define EP0_SIZE		          64
#define DEVICE_CLASS          0
#define DEVICE_SUBCLASS       0
#define DEVICE_PROTOCOL       0
#define NUM_ENDPOINTS         2
#define NUM_USB_BUFFERS	      30
#define NUM_INTERFACE		      1
#define MIDI_INTERFACE        0
#define MIDI_NUM_CABLES       3
#define MIDI_TX_ENDPOINT      1
#define MIDI_TX_SIZE          64
#define MIDI_RX_ENDPOINT      2
#define MIDI_RX_SIZE          64
#define ENDPOINT1_CONFIG	    ENDPOINT_TRANSIMIT_ONLY
#define ENDPOINT2_CONFIG	    ENDPOINT_RECEIVE_ONLY
#define USB_MIDI

#define CDC_STATUS_INTERFACE   1
#define CDC_DATA_INTERFACE     1

typedef struct {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint16_t wString[];
} usb_string_descriptor_struct_t;

typedef struct {
	uint16_t	wValue;
	uint16_t	wIndex;
	const uint8_t	*addr;
	uint16_t	length;
} usb_descriptor_list_t;

extern const uint8_t usb_endpoint_config_table[NUM_ENDPOINTS];
extern const usb_descriptor_list_t usb_descriptor_list[];

#endif
