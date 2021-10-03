#include "usb_desc.h"
#include "kinetis.h"
#include "avr_functions.h"


usb_string_descriptor_struct_t usb_string_product_name = {
	2 + MIDI_NAME_LEN * 2,
	3,
	MIDI_NAME
};

usb_string_descriptor_struct_t usb_string_manufacturer_name = {
	2 + MANUFACTURER_NAME_LEN * 2,
	3,
	MANUFACTURER_NAME
};

usb_string_descriptor_struct_t usb_string_serial_number = {
	2 + SERIAL_NUMBER_LEN * 2,
	3,
	SERIAL_NUMBER
};

usb_string_descriptor_struct_t usb_string_midi_port1 = {
	2 + MIDI_PORT1_NAME_LEN * 2,
	3,
	MIDI_PORT1_NAME
};

usb_string_descriptor_struct_t usb_string_midi_port2 = {
	2 + MIDI_PORT2_NAME_LEN * 2,
	3,
	MIDI_PORT2_NAME
};

usb_string_descriptor_struct_t usb_string_midi_port3 = {
	2 + MIDI_PORT3_NAME_LEN * 2,
	3,
	MIDI_PORT3_NAME
};


// **************************************************************
//   USB Device
// **************************************************************

#define LSB(n) ((n) & 0xFF)
#define MSB(n) (((n) >> 8) & 0xFF)
#define MIDI_INTERFACE_JACK_PAIR(a, b, c, d, e) \
6, 0x24, 0x02, 0x01, (a), (e), \
6, 0x24, 0x02, 0x02, (b), 0, \
9, 0x24, 0x03, 0x01, (c), 1, (b), 1, (e), \
9, 0x24, 0x03, 0x02, (d), 1, (a), 1, 0,

// USB Device Descriptor.  The USB host reads this first, to learn
// what type of device is connected.
static uint8_t device_descriptor[] = {
	18,                     // bLength
	1,                      // bDescriptorType
	0x10,			  0x01,       // bcdUSB
	DEVICE_CLASS,           // bDeviceClass
	DEVICE_SUBCLASS,        // bDeviceSubClass
	DEVICE_PROTOCOL,        // bDeviceProtocol
	EP0_SIZE,                           // bMaxPacketSize0
	LSB (VENDOR_ID),  MSB (VENDOR_ID),  // idVendor
	LSB (PRODUCT_ID), MSB (PRODUCT_ID), // idProduct
	LSB (BCD_DEVICE), MSB (BCD_DEVICE), // bcdDevice
	1,                  // iManufacturer
	2,                  // iProduct
	0,                  // iSerialNumber
	1                   // bNumConfigurations
};

// These descriptors must NOT be "const", because the USB DMA
// has trouble accessing flash memory with enough bandwidth
// while the processor is executing from flash.

// **************************************************************
//   USB Descriptor Sizes
// **************************************************************

// pre-compute the size and position of everything in the config descriptor
//


#define AUDIO_CONTROL 1

#ifdef AUDIO_CONTROL
#define CONFIG_HEADER_DESCRIPTOR_SIZE   9
#define AUDIO_CONTROL_DESC_POS			    CONFIG_HEADER_DESCRIPTOR_SIZE
#define AUDIO_CONTROL_DESC_SIZE			    9 + 9
#define MIDI_INTERFACE_DESC_POS         AUDIO_CONTROL_DESC_POS + AUDIO_CONTROL_DESC_SIZE
#define MIDI_INTERFACE_DESC_SIZE        9 + 7 + ((6 + 6 + 9 + 9) * MIDI_NUM_CABLES) + (9 + (4 + MIDI_NUM_CABLES) + 9 + (4 + MIDI_NUM_CABLES))
#define CONFIG_DESC_SIZE                MIDI_INTERFACE_DESC_POS + MIDI_INTERFACE_DESC_SIZE
#else
#define CONFIG_HEADER_DESCRIPTOR_SIZE   9
#define MIDI_INTERFACE_DESC_POS         0 + CONFIG_HEADER_DESCRIPTOR_SIZE
#define MIDI_INTERFACE_DESC_SIZE        9 + 7 + ((6 + 6 + 9 + 9) * MIDI_NUM_CABLES) + (9 + (4 + MIDI_NUM_CABLES) + 9 + (4 + MIDI_NUM_CABLES))
#define CONFIG_DESC_SIZE                MIDI_INTERFACE_DESC_POS + MIDI_INTERFACE_DESC_SIZE
#endif


// **************************************************************
//   USB Configuration
// **************************************************************

// USB Configuration Descriptor.  This huge descriptor tells all
// of the devices capbilities.
static uint8_t config_descriptor[CONFIG_DESC_SIZE] = {
	// configuration descriptor, USB spec 9.6.3, page 264-266, Table 9-10
	9,                                          // bLength;
	2,                                          // bDescriptorType;
	LSB (CONFIG_DESC_SIZE),                     // wTotalLength
	MSB (CONFIG_DESC_SIZE),
#ifdef AUDIO_CONTROL
	2,                                          // bNumInterfaces
#else
  1,
#endif
	1,                                          // bConfigurationValue
	0,                                          // iConfiguration
	0x80,                                       // bmAttributes
	250,                                        // bMaxPower

#ifdef AUDIO_CONTROL
	// AUDIO CONTROL Interface (Standard AC Interface Descriptor(4.7.1)
	9, 																			    // bLength(0x09)
	4, 																			    // bDescriptorType(0x04): INTERFACE
	0, 																			    // bInterfaceNumber(0x00)
	0, 																			    // bAlternateSetting(0x00)
	0, 																			    // bNumEndpoints(0x00)
	1, 																			    // bInterfaceClass(0x01): AUDIO
	1, 																			    // bInterfaceSubClass(0x01): AUDIOCONTROL
	0,    																			// bInterfaceProtocol(0x20): iProtocol
  0,                                          // iInterface

  // Audio control specific class header
	9,
	36,
	1,
	0,
	1,
	9,
	0,
	1,
	1,
#endif /* AUDIO_CONTROL */


	// Standard MS Interface Descriptor,
	9,                                                  // bLength
	4,                                                  // bDescriptorType
#ifdef AUDIO_CONTROL
	1,                                                  // bInterfaceNumber
#else
  0,
#endif
	0,                                                  // bAlternateSetting
	2,                                                  // bNumEndpoints
	0x01,                                               // bInterfaceClass (0x01 = Audio)
	0x03,                                               // bInterfaceSubClass (0x03 = MIDI)
	0x00,                                               // bInterfaceProtocol (unused for MIDI)
	0,                                                  // iInterface

	// MIDI MS Interface Header, USB MIDI 6.1.2.1, page 21, Table 6-2
	7,                                                  // bLength
	0x24,                                               // bDescriptorType = CS_INTERFACE
	0x01,                                               // bDescriptorSubtype = MS_HEADER
	0x00,										0x01,                       // bcdMSC = revision 01.00
	LSB (7 + ((6 + 6 + 9 + 9) * MIDI_NUM_CABLES) + 9 + (4 + MIDI_NUM_CABLES) + 9 + (4 + MIDI_NUM_CABLES)), // wTotalLength
	MSB (7 + ((6 + 6 + 9 + 9) * MIDI_NUM_CABLES) + 9 + (4 + MIDI_NUM_CABLES) + 9 + (4 + MIDI_NUM_CABLES)),

  // MIDI jacks definitions
	#if (MIDI_NUM_CABLES >= 1)
	MIDI_INTERFACE_JACK_PAIR (1, 2, 3, 4, 4)
	#endif
	#if (MIDI_NUM_CABLES >= 2)
	MIDI_INTERFACE_JACK_PAIR (5, 6, 7, 8, 5)
	#endif
	#if (MIDI_NUM_CABLES >= 3)
	MIDI_INTERFACE_JACK_PAIR (9, 10, 11, 12, 6)
	#endif
	#if (MIDI_NUM_CABLES >= 4)
	MIDI_INTERFACE_JACK_PAIR (13, 14, 15, 16, 6)
	#endif

	// Standard Bulk OUT Endpoint Descriptor, B.5.1, Table B-11, pae 42
	9,                                              // bLength
	5,                                              // bDescriptorType = ENDPOINT
	MIDI_RX_ENDPOINT,                               // bEndpointAddress
	0x02,                                           // bmAttributes (0x02=bulk)
	MIDI_RX_SIZE,								   0,               // wMaxPacketSize
	0,                                              // bInterval
	0,                                              // bRefresh
	0,                                              // bSynchAddress

	// Class-specific MS Bulk OUT Endpoint Descriptor, B.5.2, Table B-12, page 42
	(4 + MIDI_NUM_CABLES),                          // bLength
	0x25,                                           // bDescriptorSubtype = CS_ENDPOINT
	0x01,                                           // bJackType = MS_GENERAL
	MIDI_NUM_CABLES,                                // bNumEmbMIDIJack = number of jacks
	1,                                              // BaAssocJackID(1) = jack ID #1
#if (MIDI_NUM_CABLES >= 2)
	5,
#endif
#if (MIDI_NUM_CABLES >= 3)
	9,
#endif
#if (MIDI_NUM_CABLES >= 4)
	13,
#endif

  // this endpoint does not work
	// Standard Bulk IN Endpoint Descriptor, B.5.1, Table B-11, pae 42
	9,                                              // bLength
	5,                                              // bDescriptorType = ENDPOINT
	MIDI_TX_ENDPOINT | 0x80,                        // bEndpointAddress
	0x02,                                           // bmAttributes (0x02=bulk)
	MIDI_TX_SIZE,								0,                  // wMaxPacketSize
	0,                                              // bInterval
	0,                                              // bRefresh
	0,                                              // bSynchAddress

	// Class-specific MS Bulk IN Endpoint Descriptor, B.5.2, Table B-12, page 42
	(4 + MIDI_NUM_CABLES),                            // bLength
	0x25,                                           // bDescriptorSubtype = CS_ENDPOINT
	0x01,                                           // bJackType = MS_GENERAL
	MIDI_NUM_CABLES,                                // bNumEmbMIDIJack = number of jacks
	3,                                              // BaAssocJackID(1) = jack ID #3
#if (MIDI_NUM_CABLES >= 2)
	7,
#endif
#if (MIDI_NUM_CABLES >= 3)
	11,
#endif
#if (MIDI_NUM_CABLES >= 4)
	15,
#endif
};


// **************************************************************
//   String Descriptors
// **************************************************************

// The descriptors above can provide human readable strings,
// referenced by index numbers.  These descriptors are the
// actual string data

usb_string_descriptor_struct_t string0 = {
	4,
	3,
	{ 0x0409 }
};


// **************************************************************
//   Descriptors List
// **************************************************************

// This table provides access to all the descriptor data above.

const usb_descriptor_list_t usb_descriptor_list[] = {
	//wValue, wIndex, address,          length
	{ 0x0100, 0x0000, device_descriptor, sizeof (device_descriptor) },
	{ 0x0200, 0x0000, config_descriptor, sizeof (config_descriptor)	},
	{ 0x0300, 0x0000,	(const uint8_t *) &string0, 0 },
	{ 0x0301, 0x0409,	(const uint8_t *) &usb_string_manufacturer_name, 0 },
	{ 0x0302, 0x0409,	(const uint8_t *) &usb_string_product_name, 0 },
	{ 0x0303, 0x0409,	(const uint8_t *) &usb_string_serial_number, 0 },
	{ 0x0304, 0x0409,	(const uint8_t *) &usb_string_midi_port1,	0 },
	{ 0x0305, 0x0409,	(const uint8_t *) &usb_string_midi_port2,	0 },
	{ 0x0306, 0x0409,	(const uint8_t *) &usb_string_midi_port3,	0 },
	{ 0, 0, NULL, 0 }
};


// **************************************************************
//   Endpoint Configuration
// **************************************************************


const uint8_t usb_endpoint_config_table[NUM_ENDPOINTS] =
{
	ENDPOINT1_CONFIG,
	ENDPOINT2_CONFIG
};
