#include "usb_desc.h"
#include "mk66fx1m0.h"

uint8_t NUM_INTERFACE = 2;
uint8_t NUM_ENDPOINTS = 3;
uint8_t KEYBOARD_INTERFACE = 0;
uint8_t KEYBOARD_ENDPOINT = 0;
uint8_t MOUSE_INTERFACE = 0;
uint8_t MOUSE_ENDPOINT = 0;
uint8_t KEYMEDIA_INTERFACE = 0;
uint8_t KEYMEDIA_ENDPOINT = 0;

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

#define MIDI_INTERFACE_JACK_PAIR(a, b, c, d, e)                                \
    6, 0x24, 0x02, 0x01, (a), (e), 6, 0x24, 0x02, 0x02, (b), 0, 9, 0x24, 0x03, \
        0x01, (c), 1, (b), 1, (e), 9, 0x24, 0x03, 0x02, (d), 1, (a), 1, 0,

// USB Device Descriptor.  The USB host reads this first, to learn
// what type of device is connected.
static uint8_t device_descriptor[] = {
    18, // bLength
    1, // bDescriptorType
    0x10,
    0x01, // bcdUSB
    DEVICE_CLASS, // bDeviceClass
    DEVICE_SUBCLASS, // bDeviceSubClass
    DEVICE_PROTOCOL, // bDeviceProtocol
    EP0_SIZE, // bMaxPacketSize0
    LSB(VENDOR_ID),
    MSB(VENDOR_ID), // idVendor
    LSB(PRODUCT_ID),
    MSB(PRODUCT_ID), // idProduct
    LSB(BCD_DEVICE),
    MSB(BCD_DEVICE), // bcdDevice
    1, // iManufacturer
    2, // iProduct
    0, // iSerialNumber
    1 // bNumConfigurations
};

static uint8_t keyboard_report_desc[] = {
    0x05, 0x01, // Usage Page (Generic Desktop),
    0x09, 0x06, // Usage (Keyboard),
    0xA1, 0x01, // Collection (Application),
    0x75, 0x01, //   Report Size (1),
    0x95, 0x08, //   Report Count (8),
    0x05, 0x07, //   Usage Page (Key Codes),
    0x19, 0xE0, //   Usage Minimum (224),
    0x29, 0xE7, //   Usage Maximum (231),
    0x15, 0x00, //   Logical Minimum (0),
    0x25, 0x01, //   Logical Maximum (1),
    0x81, 0x02, //   Input (Data, Variable, Absolute), ;Modifier keys
    0x95, 0x01, //   Report Count (1),
    0x75, 0x08, //   Report Size (8),
    0x81, 0x03, //   Input (Constant),          ;Reserved byte
    0x95, 0x05, //   Report Count (5),
    0x75, 0x01, //   Report Size (1),
    0x05, 0x08, //   Usage Page (LEDs),
    0x19, 0x01, //   Usage Minimum (1),
    0x29, 0x05, //   Usage Maximum (5),
    0x91, 0x02, //   Output (Data, Variable, Absolute), ;LED report
    0x95, 0x01, //   Report Count (1),
    0x75, 0x03, //   Report Size (3),
    0x91, 0x03, //   Output (Constant),         ;LED report padding
    0x95, 0x06, //   Report Count (6),
    0x75, 0x08, //   Report Size (8),
    0x15, 0x00, //   Logical Minimum (0),
    0x25, 0x7F, //   Logical Maximum(104),
    0x05, 0x07, //   Usage Page (Key Codes),
    0x19, 0x00, //   Usage Minimum (0),
    0x29, 0x7F, //   Usage Maximum (104),
    0x81, 0x00, //   Input (Data, Array),       ;Normal keys
    0xC0 // End Collection
};

static uint8_t keymedia_report_desc[] = {
    0x05, 0x0C, // Usage Page (Consumer)
    0x09, 0x01, // Usage (Consumer Controls)
    0xA1, 0x01, // Collection (Application)
    0x75, 0x0A, //   Report Size (10)
    0x95, 0x04, //   Report Count (4)
    0x19, 0x00, //   Usage Minimum (0)
    0x2A, 0x9C, 0x02, //   Usage Maximum (0x29C)
    0x15, 0x00, //   Logical Minimum (0)
    0x26, 0x9C, 0x02, //   Logical Maximum (0x29C)
    0x81, 0x00, //   Input (Data, Array)
    0x05, 0x01, //   Usage Page (Generic Desktop)
    0x75, 0x08, //   Report Size (8)
    0x95, 0x03, //   Report Count (3)
    0x19, 0x00, //   Usage Minimum (0)
    0x29, 0xB7, //   Usage Maximum (0xB7)
    0x15, 0x00, //   Logical Minimum (0)
    0x26, 0xB7, 0x00, //   Logical Maximum (0xB7)
    0x81, 0x00, //   Input (Data, Array)
    0xC0 // End Collection
};

static uint8_t mouse_report_desc[] = {
    0x05, 0x01, // Usage Page (Generic Desktop)
    0x09, 0x02, // Usage (Mouse)
    0xA1, 0x01, // Collection (Application)
    0x85, 0x01, //   REPORT_ID (1)
    0x05, 0x09, //   Usage Page (Button)
    0x19, 0x01, //   Usage Minimum (Button #1)
    0x29, 0x08, //   Usage Maximum (Button #8)
    0x15, 0x00, //   Logical Minimum (0)
    0x25, 0x01, //   Logical Maximum (1)
    0x95, 0x08, //   Report Count (8)
    0x75, 0x01, //   Report Size (1)
    0x81, 0x02, //   Input (Data, Variable, Absolute)
    0x05, 0x01, //   Usage Page (Generic Desktop)
    0x09, 0x30, //   Usage (X)
    0x09, 0x31, //   Usage (Y)
    0x09, 0x38, //   Usage (Wheel)
    0x15, 0x81, //   Logical Minimum (-127)
    0x25, 0x7F, //   Logical Maximum (127)
    0x75, 0x08, //   Report Size (8),
    0x95, 0x03, //   Report Count (3),
    0x81, 0x06, //   Input (Data, Variable, Relative)
    0x05, 0x0C, //   Usage Page (Consumer)
    0x0A, 0x38, 0x02, //   Usage (AC Pan)
    0x15, 0x81, //   Logical Minimum (-127)
    0x25, 0x7F, //   Logical Maximum (127)
    0x75, 0x08, //   Report Size (8),
    0x95, 0x01, //   Report Count (1),
    0x81, 0x06, //   Input (Data, Variable, Relative)
    0xC0, // End Collection
    0x05, 0x01, // Usage Page (Generic Desktop)
    0x09, 0x02, // Usage (Mouse)
    0xA1, 0x01, // Collection (Application)
    0x85, 0x02, //   REPORT_ID (2)
    0x05, 0x01, //   Usage Page (Generic Desktop)
    0x09, 0x30, //   Usage (X)
    0x09, 0x31, //   Usage (Y)
    0x15, 0x00, //   Logical Minimum (0)
    0x26, 0xFF, 0x7F, //   Logical Maximum (32767)
    0x75, 0x10, //   Report Size (16),
    0x95, 0x02, //   Report Count (2),
    0x81, 0x02, //   Input (Data, Variable, Absolute)
    0xC0 // End Collection
};

// These descriptors must NOT be "const", because the USB DMA
// has trouble accessing flash memory with enough bandwidth
// while the processor is executing from flash.

// **************************************************************
//   USB Descriptor Sizes
// **************************************************************

uint8_t config_descriptor_keyboard[KEYBOARD_INTERFACE_DESC_SIZE] = {
    9, // bLength
    4, // bDescriptorType
    0, // bInterfaceNumber
    0, // bAlternateSetting
    1, // bNumEndpoints
    0x03, // bInterfaceClass (0x03 = HID)
    0x01, // bInterfaceSubClass (0x01 = Boot)
    0x01, // bInterfaceProtocol (0x01 = Keyboard)
    0, // iInterface
    // HID interface descriptor, HID 1.11 spec, section 6.2.1
    9, // bLength
    0x21, // bDescriptorType
    0x11,
    0x01, // bcdHID
    0, // bCountryCode
    1, // bNumDescriptors
    0x22, // bDescriptorType
    LSB(sizeof(keyboard_report_desc)), // wDescriptorLength
    MSB(sizeof(keyboard_report_desc)),
    // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
    7, // bLength
    5, // bDescriptorType
    0, // bEndpointAddress
    0x03, // bmAttributes (0x03=intr)
    KEYBOARD_SIZE,
    0, // wMaxPacketSize
    KEYBOARD_INTERVAL, // bInterval
};

uint8_t config_descriptor_mouse[MOUSE_INTERFACE_DESC_SIZE] = {
    9, // bLength
    4, // bDescriptorType
    0, // bInterfaceNumber
    0, // bAlternateSetting
    1, // bNumEndpoints
    0x03, // bInterfaceClass (0x03 = HID)
    0x00, // bInterfaceSubClass (0x01 = Boot)
    0x00, // bInterfaceProtocol (0x02 = Mouse)
    0, // iInterface
    // HID interface descriptor, HID 1.11 spec, section 6.2.1
    9, // bLength
    0x21, // bDescriptorType
    0x11,
    0x01, // bcdHID
    0, // bCountryCode
    1, // bNumDescriptors
    0x22, // bDescriptorType
    LSB(sizeof(mouse_report_desc)), // wDescriptorLength
    MSB(sizeof(mouse_report_desc)),
    // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
    7, // bLength
    5, // bDescriptorType
    0, // bEndpointAddress
    0x03, // bmAttributes (0x03=intr)
    MOUSE_SIZE,
    0, // wMaxPacketSize
    MOUSE_INTERVAL, // bInterval
};

uint8_t config_descriptor_keymedia[KEYMEDIA_INTERFACE_DESC_SIZE] = {
    // interface descriptor, USB spec 9.6.5, page 267-269, Table 9-12
    9, // bLength
    4, // bDescriptorType
    0, // bInterfaceNumber
    0, // bAlternateSetting
    1, // bNumEndpoints
    0x03, // bInterfaceClass (0x03 = HID)
    0x00, // bInterfaceSubClass
    0x00, // bInterfaceProtocol
    0, // iInterface
    // HID interface descriptor, HID 1.11 spec, section 6.2.1
    9, // bLength
    0x21, // bDescriptorType
    0x11,
    0x01, // bcdHID
    0, // bCountryCode
    1, // bNumDescriptors
    0x22, // bDescriptorType
    LSB(sizeof(keymedia_report_desc)), // wDescriptorLength
    MSB(sizeof(keymedia_report_desc)),
    // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
    7, // bLength
    5, // bDescriptorType
    0, // bEndpointAddress
    0x03, // bmAttributes (0x03=intr)
    KEYMEDIA_SIZE,
    0, // wMaxPacketSize
    KEYMEDIA_INTERVAL, // bInterval
};

// **************************************************************
//   USB Configuration
// **************************************************************

// USB Configuration Descriptor.  This huge descriptor tells all
// of the devices capbilities.
uint8_t config_descriptor[CONFIG_DESC_SIZE] = {
    // configuration descriptor, USB spec 9.6.3, page 264-266, Table 9-10
    9, // bLength;
    2, // bDescriptorType;
    LSB(MINIMUM_CONFIG_SIZE), // wTotalLength
    MSB(MINIMUM_CONFIG_SIZE),
    2, // bNumInterfaces
    1, // bConfigurationValue
    0, // iConfiguration
    0x80, // bmAttributes
    250, // bMaxPower

    // AUDIO CONTROL Interface (Standard AC Interface Descriptor(4.7.1)
    9, // bLength(0x09)
    4, // bDescriptorType(0x04): INTERFACE
    0, // bInterfaceNumber(0x00)
    0, // bAlternateSetting(0x00)
    0, // bNumEndpoints(0x00)
    1, // bInterfaceClass(0x01): AUDIO
    1, // bInterfaceSubClass(0x01): AUDIOCONTROL
    0, // bInterfaceProtocol(0x20): iProtocol
    0, // iInterface

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

    // Standard MS Interface Descriptor,
    9, // bLength
    4, // bDescriptorType
    1, // bInterfaceNumber
    0, // bAlternateSetting
    2, // bNumEndpoints
    0x01, // bInterfaceClass (0x01 = Audio)
    0x03, // bInterfaceSubClass (0x03 = MIDI)
    0x00, // bInterfaceProtocol (unused for MIDI)
    0, // iInterface

    // MIDI MS Interface Header, USB MIDI 6.1.2.1, page 21, Table 6-2
    7, // bLength
    0x24, // bDescriptorType = CS_INTERFACE
    0x01, // bDescriptorSubtype = MS_HEADER
    0x00,
    0x01, // bcdMSC = revision 01.00
    LSB(7 + ((6 + 6 + 9 + 9) * MIDI_NUM_CABLES) + 9 + (4 + MIDI_NUM_CABLES) + 9
        + (4 + MIDI_NUM_CABLES)), // wTotalLength
    MSB(7 + ((6 + 6 + 9 + 9) * MIDI_NUM_CABLES) + 9 + (4 + MIDI_NUM_CABLES) + 9
        + (4 + MIDI_NUM_CABLES)),

// MIDI jacks definitions
#if (MIDI_NUM_CABLES >= 1)
    MIDI_INTERFACE_JACK_PAIR(1, 2, 3, 4, 4)
#endif
#if (MIDI_NUM_CABLES >= 2)
        MIDI_INTERFACE_JACK_PAIR(5, 6, 7, 8, 5)
#endif
#if (MIDI_NUM_CABLES >= 3)
            MIDI_INTERFACE_JACK_PAIR(9, 10, 11, 12, 6)
#endif
#if (MIDI_NUM_CABLES >= 4)
                MIDI_INTERFACE_JACK_PAIR(13, 14, 15, 16, 6)
#endif

    // Standard Bulk OUT Endpoint Descriptor, B.5.1, Table B-11, pae 42
    9, // bLength
    5, // bDescriptorType = ENDPOINT
    MIDI_RX_ENDPOINT, // bEndpointAddress
    0x02, // bmAttributes (0x02=bulk)
    MIDI_RX_SIZE,
    0, // wMaxPacketSize
    0, // bInterval
    0, // bRefresh
    0, // bSynchAddress

    // Class-specific MS Bulk OUT Endpoint Descriptor, B.5.2, Table B-12, page 42
    (4 + MIDI_NUM_CABLES), // bLength
    0x25, // bDescriptorSubtype = CS_ENDPOINT
    0x01, // bJackType = MS_GENERAL
    MIDI_NUM_CABLES, // bNumEmbMIDIJack = number of jacks
    1, // BaAssocJackID(1) = jack ID #1
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
    9, // bLength
    5, // bDescriptorType = ENDPOINT
    MIDI_TX_ENDPOINT | 0x80, // bEndpointAddress
    0x02, // bmAttributes (0x02=bulk)
    MIDI_TX_SIZE,
    0, // wMaxPacketSize
    0, // bInterval
    0, // bRefresh
    0, // bSynchAddress

    // Class-specific MS Bulk IN Endpoint Descriptor, B.5.2, Table B-12, page 42
    (4 + MIDI_NUM_CABLES), // bLength
    0x25, // bDescriptorSubtype = CS_ENDPOINT
    0x01, // bJackType = MS_GENERAL
    MIDI_NUM_CABLES, // bNumEmbMIDIJack = number of jacks
    3, // BaAssocJackID(1) = jack ID #3
#if (MIDI_NUM_CABLES >= 2)
    7,
#endif
#if (MIDI_NUM_CABLES >= 3)
    11,
#endif
#if (MIDI_NUM_CABLES >= 4)
    15,
#endif

    // interface descriptor, USB spec 9.6.5, page 267-269, Table 9-12
    0, // bLength
    0, // bDescriptorType
    0, // bInterfaceNumber
    0, // bAlternateSetting
    0, // bNumEndpoints
    0, // bInterfaceClass (0x03 = HID)
    0, // bInterfaceSubClass (0x01 = Boot)
    0, // bInterfaceProtocol (0x01 = Keyboard)
    0, // iInterface
    // HID interface descriptor, HID 1.11 spec, section 6.2.1
    0, // bLength
    0, // bDescriptorType
    0,
    0, // bcdHID
    0, // bCountryCode
    0, // bNumDescriptors
    0, // bDescriptorType
    0, // wDescriptorLength
    0,
    // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
    0, // bLength
    0, // bDescriptorType
    0, // bEndpointAddress
    0, // bmAttributes (0x03=intr)
    0,
    0, // wMaxPacketSize
    0, // bInterval

    // interface descriptor, USB spec 9.6.5, page 267-269, Table 9-12
    0, // bLength
    0, // bDescriptorType
    0, // bInterfaceNumber
    0, // bAlternateSetting
    0, // bNumEndpoints
    0, // bInterfaceClass (0x03 = HID)
    0, // bInterfaceSubClass (0x01 = Boot)
    0, // bInterfaceProtocol (0x02 = Mouse)
    0, // iInterface
    // HID interface descriptor, HID 1.11 spec, section 6.2.1
    0, // bLength
    0, // bDescriptorType
    0,
    0, // bcdHID
    0, // bCountryCode
    0, // bNumDescriptors
    0, // bDescriptorType
    0, // wDescriptorLength
    0,
    // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
    0, // bLength
    0, // bDescriptorType
    0, // bEndpointAddress
    0, // bmAttributes (0x03=intr)
    0,
    0, // wMaxPacketSize
    0, // bInterval

    // interface descriptor, USB spec 9.6.5, page 267-269, Table 9-12
    0, // bLength
    0, // bDescriptorType
    0, // bInterfaceNumber
    0, // bAlternateSetting
    0, // bNumEndpoints
    0, // bInterfaceClass (0x03 = HID)
    0, // bInterfaceSubClass
    0, // bInterfaceProtocol
    0, // iInterface
    // HID interface descriptor, HID 1.11 spec, section 6.2.1
    0, // bLength
    0, // bDescriptorType
    0,
    0, // bcdHID
    0, // bCountryCode
    0, // bNumDescriptors
    0, // bDescriptorType
    0, // wDescriptorLength
    0,
    // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
    0, // bLength
    0, // bDescriptorType
    0, // bEndpointAddress
    0, // bmAttributes (0x03=intr)
    0,
    0, // wMaxPacketSize
    0, // bInterval
};

// **************************************************************
//   String Descriptors
// **************************************************************

// The descriptors above can provide human readable strings,
// referenced by index numbers.  These descriptors are the
// actual string data

usb_string_descriptor_struct_t string0 = { 4, 3, { 0x0409 } };

// **************************************************************
//   Descriptors List
// **************************************************************

// This table provides access to all the descriptor data above.

usb_descriptor_list_t usb_descriptor_list[] = {
    //wValue, wIndex, address,          length
    { 0x0100, 0x0000, device_descriptor, sizeof(device_descriptor) },
    { 0x0200, 0x0000, config_descriptor, 0 },
    { 0x2200,
      0, // overwriten by the configuration
      keyboard_report_desc,
      sizeof(keyboard_report_desc) },
    { 0x2100,
      0, // overwriten by the configuration
      config_descriptor, // overwriten by the configuration
      9 },
    { 0x2200,
      0, // overwriten by the configuration
      mouse_report_desc,
      sizeof(mouse_report_desc) },
    { 0x2100,
      0, // overwriten by the configuration
      config_descriptor, // overwriten by the configuration
      9 },
    { 0x2200,
      0, // overwriten by the configuration
      keymedia_report_desc,
      sizeof(keymedia_report_desc) },
    { 0x2100,
      0, // overwriten by the configuration
      config_descriptor, // overwriten by the configuration
      9 },
    { 0x0300, 0x0000, (const uint8_t *)&string0, 0 },
    { 0x0301, 0x0409, (const uint8_t *)&usb_string_manufacturer_name, 0 },
    { 0x0302, 0x0409, (const uint8_t *)&usb_string_product_name, 0 },
    { 0x0303, 0x0409, (const uint8_t *)&usb_string_serial_number, 0 },
    { 0x0304, 0x0409, (const uint8_t *)&usb_string_midi_port1, 0 },
    { 0x0305, 0x0409, (const uint8_t *)&usb_string_midi_port2, 0 },
    { 0x0306, 0x0409, (const uint8_t *)&usb_string_midi_port3, 0 },
    { 0, 0, NULL, 0 }
};

// **************************************************************
//   Endpoint Configuration
// **************************************************************

uint8_t usb_endpoint_config_table[MAX_NUM_ENDPOINTS] = { ENDPOINT1_CONFIG,
                                                         ENDPOINT2_CONFIG,
                                                         ENDPOINT_UNUSED,
                                                         ENDPOINT_UNUSED,
                                                         ENDPOINT_UNUSED };
