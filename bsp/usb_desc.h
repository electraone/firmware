#ifndef _usb_desc_h_
#define _usb_desc_h_

#include <stdint.h>
#include <stddef.h>

#define LSB(n) ((n)&0xFF)
#define MSB(n) (((n) >> 8) & 0xFF)

#define MIDI_NAME                                                              \
    {                                                                          \
        'E', 'l', 'e', 'c', 't', 'r', 'a', ' ', 'C', 'o', 'n', 't', 'r', 'o',  \
            'l', 'l', 'e', 'r'                                                 \
    }
#define MIDI_NAME_LEN 18
#define MANUFACTURER_NAME                                                      \
    {                                                                          \
        'e', 'l', 'e', 'c', 't', 'r', 'a', '.', 'o', 'n', 'e'                  \
    }
#define MANUFACTURER_NAME_LEN 11
#define SERIAL_NUMBER                                                          \
    {                                                                          \
        '1', '2', '3', '4', '1', '2', '3', '4', '5', '6'                       \
    }
#define SERIAL_NUMBER_LEN 10
#define MIDI_PORT1_NAME                                                        \
    {                                                                          \
        'E', 'l', 'e', 'c', 't', 'r', 'a', ' ', 'P', 'o', 'r', 't', ' ', '1'   \
    }
#define MIDI_PORT1_NAME_LEN 14
#define MIDI_PORT2_NAME                                                        \
    {                                                                          \
        'E', 'l', 'e', 'c', 't', 'r', 'a', ' ', 'P', 'o', 'r', 't', ' ', '2'   \
    }
#define MIDI_PORT2_NAME_LEN 14
#define MIDI_PORT3_NAME                                                        \
    {                                                                          \
        'E', 'l', 'e', 'c', 't', 'r', 'a', ' ', 'C', 'T', 'R', 'L'             \
    }
#define MIDI_PORT3_NAME_LEN 12

#define ENDPOINT_UNUSED 0x00
#define ENDPOINT_TRANSMIT_ONLY 0x15
#define ENDPOINT_RECEIVE_ONLY 0x19
#define ENDPOINT_TRANSMIT_AND_RECEIVE 0x1D
#define ENDPOINT_RECEIVE_ISOCHRONOUS 0x18
#define ENDPOINT_TRANSMIT_ISOCHRONOUS 0x14

extern uint8_t NUM_INTERFACE;
extern uint8_t NUM_ENDPOINTS;
extern uint8_t KEYBOARD_INTERFACE;
extern uint8_t KEYBOARD_ENDPOINT;
extern uint8_t MOUSE_INTERFACE;
extern uint8_t MOUSE_ENDPOINT;
extern uint8_t KEYMEDIA_INTERFACE;
extern uint8_t KEYMEDIA_ENDPOINT;

#define VENDOR_ID 0x1FC9
#define PRODUCT_ID 0x82CF
#define BCD_DEVICE 0x0213
#define EP0_ENDPOINT 0
#define EP0_SIZE 64
#define DEVICE_CLASS 0
#define DEVICE_SUBCLASS 0
#define DEVICE_PROTOCOL 0
#define MAX_NUM_INTERFACE 5
#define MAX_NUM_ENDPOINTS 5
#define NUM_USB_BUFFERS 30

#define MIDI_INTERFACE 0
#define MIDI_NUM_CABLES 3
#define MIDI_TX_ENDPOINT 1
#define MIDI_TX_SIZE 64
#define MIDI_RX_ENDPOINT 2
#define MIDI_RX_SIZE 64

//#define KEYBOARD_INTERFACE 2
//#define KEYBOARD_ENDPOINT 3
#define KEYBOARD_SIZE 8
#define KEYBOARD_INTERVAL 1

//#define MOUSE_INTERFACE 3
//#define MOUSE_ENDPOINT 4
#define MOUSE_SIZE 8
#define MOUSE_INTERVAL 1

//#define KEYMEDIA_INTERFACE 4
//#define KEYMEDIA_ENDPOINT 5
#define KEYMEDIA_SIZE 8
#define KEYMEDIA_INTERVAL 4

#define ENDPOINT1_CONFIG ENDPOINT_TRANSMIT_ONLY
#define ENDPOINT2_CONFIG ENDPOINT_RECEIVE_ONLY

#define USB_MIDI

#define CDC_STATUS_INTERFACE 5
#define CDC_DATA_INTERFACE 5

typedef struct {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint16_t wString[];
} usb_string_descriptor_struct_t;

typedef struct {
    uint16_t wValue;
    uint16_t wIndex;
    const uint8_t *addr;
    uint16_t length;
} usb_descriptor_list_t;

// pre-compute the size and position of everything in the config descriptor

#define CONFIG_HEADER_DESCRIPTOR_SIZE 9
#define AUDIO_CONTROL_DESC_POS CONFIG_HEADER_DESCRIPTOR_SIZE
#define AUDIO_CONTROL_DESC_SIZE 9 + 9
#define MIDI_INTERFACE_DESC_POS AUDIO_CONTROL_DESC_POS + AUDIO_CONTROL_DESC_SIZE
#define MIDI_INTERFACE_DESC_SIZE                                               \
    9 + 7 + ((6 + 6 + 9 + 9) * MIDI_NUM_CABLES)                                \
        + (9 + (4 + MIDI_NUM_CABLES) + 9 + (4 + MIDI_NUM_CABLES))

#define MINIMUM_CONFIG_SIZE MIDI_INTERFACE_DESC_POS + MIDI_INTERFACE_DESC_SIZE

#define KEYBOARD_INTERFACE_DESC_SIZE 9 + 9 + 7
#define MOUSE_INTERFACE_DESC_SIZE 9 + 9 + 7
#define KEYMEDIA_INTERFACE_DESC_SIZE 9 + 9 + 7

#define CONFIG_DESC_SIZE                                                       \
    MINIMUM_CONFIG_SIZE + KEYBOARD_INTERFACE_DESC_SIZE                         \
        + MOUSE_INTERFACE_DESC_SIZE + KEYMEDIA_INTERFACE_DESC_SIZE

extern uint8_t usb_endpoint_config_table[MAX_NUM_ENDPOINTS];
extern usb_descriptor_list_t usb_descriptor_list[];
extern uint8_t config_descriptor_keyboard[KEYBOARD_INTERFACE_DESC_SIZE];
extern uint8_t config_descriptor_mouse[MOUSE_INTERFACE_DESC_SIZE];
extern uint8_t config_descriptor_keymedia[KEYMEDIA_INTERFACE_DESC_SIZE];
extern uint8_t config_descriptor[CONFIG_DESC_SIZE];

#endif
