#include "usb_dev.h"

#include "kinetis.h"
#include "usb_mem.h"
#include <string.h> // for memset

extern void printText(const char *format, ...);
extern void printChar(char c);

/*
 * Interrupt status
 */
volatile int rxIsrDisabled;

/*
 * Buffer descriptor table
 */
typedef struct {
    uint32_t desc;
    void *addr;
} bdt_t;

__attribute__((section(".usbdescriptortable"),
               used)) static bdt_t table[(MAX_NUM_ENDPOINTS + 1) * 4];

/*
 * Per Endpoint buffer memory
 */
static usb_packet_t *rx_first[MAX_NUM_ENDPOINTS];
static usb_packet_t *rx_last[MAX_NUM_ENDPOINTS];
static usb_packet_t *tx_first[MAX_NUM_ENDPOINTS];
static usb_packet_t *tx_last[MAX_NUM_ENDPOINTS];
uint16_t usb_rx_byte_count_data;

/*
 * State of the TX flow
 */
static uint8_t tx_state[MAX_NUM_ENDPOINTS];
#define TX_STATE_BOTH_FREE_EVEN_FIRST 0
#define TX_STATE_BOTH_FREE_ODD_FIRST 1
#define TX_STATE_EVEN_FREE 2
#define TX_STATE_ODD_FREE 3
#define TX_STATE_NONE_FREE_EVEN_FIRST 4
#define TX_STATE_NONE_FREE_ODD_FIRST 5

/*
 * BD status bits
 */
#define BDT_OWN 0x80
#define BDT_DATA1 0x40
#define BDT_DATA0 0x00
#define BDT_DTS 0x08
#define BDT_STALL 0x04

#define BDT_PID(n) (((n) >> 2) & 15)
#define BDT_DESC(count, data)                                                  \
    (BDT_OWN | BDT_DTS | ((data) ? BDT_DATA1 : BDT_DATA0) | ((count) << 16))

/*
 * Flags for the BD adressing
 */
#define TX 1
#define RX 0
#define ODD 1
#define EVEN 0
#define DATA0 0
#define DATA1 1

#define index(endpoint, tx, odd) (((endpoint) << 2) | ((tx) << 1) | (odd))
#define stat2bufferdescriptor(stat) (table + ((stat) >> 2))

/*
 * Setup request structure
 */
static union {
    struct {
        union {
            struct {
                uint8_t bmRequestType;
                uint8_t bRequest;
            };
            uint16_t wRequestAndType;
        };
        uint16_t wValue;
        uint16_t wIndex;
        uint16_t wLength;
    };
    struct {
        uint32_t word1;
        uint32_t word2;
    };
} setup;

/*
 * Setup commands
 */
#define GET_STATUS 0
#define CLEAR_FEATURE 1
#define SET_FEATURE 3
#define SET_ADDRESS 5
#define GET_DESCRIPTOR 6
#define SET_DESCRIPTOR 7
#define GET_CONFIGURATION 8
#define SET_CONFIGURATION 9
#define GET_INTERFACE 10
#define SET_INTERFACE 11
#define SYNCH_FRAME 12

// SETUP always uses a DATA0 PID for the data field of the SETUP transaction.
// transactions in the data phase start with DATA1 and toggle (figure 8-12, USB1.1)
// Status stage uses a DATA1 PID.

static uint8_t ep0_rx0_buf[EP0_SIZE] __attribute__((aligned(4)));
static uint8_t ep0_rx1_buf[EP0_SIZE] __attribute__((aligned(4)));
static const uint8_t *ep0_tx_ptr = NULL;
static uint16_t ep0_tx_len;
static uint8_t ep0_tx_bdt_bank = 0;
static uint8_t ep0_tx_data_toggle = 0;
uint8_t usb_rx_memory_needed = 0;

volatile uint8_t usb_configuration = 0;
volatile uint8_t usb_reboot_timer = 0;
static uint8_t reply_buffer[8];

static void endpoint0_stall(void)
{
    USB0_ENDPT0 = USB_ENDPT_EPSTALL | USB_ENDPT_EPRXEN | USB_ENDPT_EPTXEN
                  | USB_ENDPT_EPHSHK;
}

static void endpoint0_transmit(const void *data, uint32_t len)
{
    table[index(0, TX, ep0_tx_bdt_bank)].addr = (void *)data;
    table[index(0, TX, ep0_tx_bdt_bank)].desc =
        BDT_DESC(len, ep0_tx_data_toggle);
    ep0_tx_data_toggle ^= 1;
    ep0_tx_bdt_bank ^= 1;
}

static void usb_setup(void)
{
    const uint8_t *data = NULL;
    uint32_t datalen = 0;
    const usb_descriptor_list_t *list;
    uint32_t size;
    volatile uint8_t *reg;
    uint8_t epconf;
    const uint8_t *cfg;
    int i;

    // printText ("wRequestAndType: %04X, wVaulue: %04X", setup.wRequestAndType, setup.wValue);

    switch (setup.wRequestAndType) {
        case 0x0500: // SET_ADDRESS
            // printText ("SET ADDRESS: %d", setup.wValue);
            break;

        case 0x0900: // SET_CONFIGURATION
            // printText ("SET CONFIGURATION: %d", setup.wValue);
            usb_configuration = setup.wValue;
            reg = &USB0_ENDPT1;
            cfg = usb_endpoint_config_table;

            // clear all BDT entries, free any allocated memory...
            for (i = 4; i < (NUM_ENDPOINTS + 1) * 4; i++) {
                if (table[i].desc & BDT_OWN) {
                    usb_free((usb_packet_t *)((uint8_t *)(table[i].addr) - 8));
                }
            }

            // free all queued packets
            for (i = 0; i < NUM_ENDPOINTS; i++) {
                usb_packet_t *p, *n;

                /*
			 * Clean up RX buffers
			 */
                p = rx_first[i];
                while (p) {
                    n = p->next;
                    usb_free(p);
                    p = n;
                }
                rx_first[i] = NULL;
                rx_last[i] = NULL;

                p = tx_first[i];
                while (p) {
                    n = p->next;
                    usb_free_tx(p);
                    p = n;
                }
                tx_first[i] = NULL;
                tx_last[i] = NULL;

                usb_rx_byte_count_data = 0;

                switch (tx_state[i]) {
                    case TX_STATE_EVEN_FREE:
                    case TX_STATE_NONE_FREE_EVEN_FIRST:
                        tx_state[i] = TX_STATE_BOTH_FREE_EVEN_FIRST;
                        break;
                    case TX_STATE_ODD_FREE:
                    case TX_STATE_NONE_FREE_ODD_FIRST:
                        tx_state[i] = TX_STATE_BOTH_FREE_ODD_FIRST;
                        break;
                    default:
                        break;
                }
            }
            usb_rx_memory_needed = 0;

            for (i = 1; i <= NUM_ENDPOINTS; i++) {
                epconf = *cfg++;
                *reg = epconf;
                reg += 4;

                if (epconf & USB_ENDPT_EPRXEN) {
                    usb_packet_t *p;
                    p = usb_malloc();
                    if (p) {
                        table[index(i, RX, EVEN)].addr = p->buf;
                        table[index(i, RX, EVEN)].desc = BDT_DESC(64, 0);
                    } else {
                        table[index(i, RX, EVEN)].desc = 0;
                        usb_rx_memory_needed++;
                    }
                    p = usb_malloc();
                    if (p) {
                        table[index(i, RX, ODD)].addr = p->buf;
                        table[index(i, RX, ODD)].desc = BDT_DESC(64, 1);
                    } else {
                        table[index(i, RX, ODD)].desc = 0;
                        usb_rx_memory_needed++;
                    }
                }
                table[index(i, TX, EVEN)].desc = 0;
                table[index(i, TX, ODD)].desc = 0;
            }
            break;

        case 0x0880: // GET_CONFIGURATION
            reply_buffer[0] = usb_configuration;
            datalen = 1;
            data = reply_buffer;
            break;

        case 0x0080: // GET_STATUS (device)
            reply_buffer[0] = 0;
            reply_buffer[1] = 0;
            datalen = 2;
            data = reply_buffer;
            break;

        case 0x0082: // GET_STATUS (endpoint)
            i = setup.wIndex & 0x7F;
            if (i > NUM_ENDPOINTS) {
                // \todo do we need to handle IN vs OUT here?
                endpoint0_stall();
                return;
            }
            reply_buffer[0] = 0;
            reply_buffer[1] = 0;
            if (*(uint8_t *)(&USB0_ENDPT0 + i * 4) & 0x02) {
                reply_buffer[0] = 1;
            }
            data = reply_buffer;
            datalen = 2;
            break;

        case 0x0102: // CLEAR_FEATURE (endpoint)
            i = setup.wIndex & 0x7F;
            if (i > NUM_ENDPOINTS || setup.wValue != 0) {
                // \todo do we need to handle IN vs OUT here?
                endpoint0_stall();
                return;
            }
            (*(uint8_t *)(&USB0_ENDPT0 + i * 4)) &= ~0x02;
            // \todo do we need to clear the data toggle here?
            break;

        case 0x0302: // SET_FEATURE (endpoint)
            i = setup.wIndex & 0x7F;
            if (i > NUM_ENDPOINTS || setup.wValue != 0) {
                // \todo do we need to handle IN vs OUT here?
                endpoint0_stall();
                return;
            }
            (*(uint8_t *)(&USB0_ENDPT0 + i * 4)) |= 0x02;
            // \todo do we need to clear the data toggle here?
            break;

        case 0x0680: // GET_DESCRIPTOR
        case 0x0681:
            // printText ("GET DESC: wValue=%04X, wIndex=%04X", setup.wValue, setup.wIndex);

            for (list = usb_descriptor_list; 1; list++) {
                if (list->addr == NULL) {
                    break;
                }

                if (setup.wValue == list->wValue
                    && setup.wIndex == list->wIndex) {
                    data = list->addr;

                    if ((setup.wValue >> 8) == 3) {
                        datalen = *(list->addr);
                    } else {
                        datalen = list->length;
                    }

                    goto send;
                }
            }

            //serial_print("desc: not found\n");
            endpoint0_stall();
            return;

        default:
            endpoint0_stall();
            return;
    }

send:

    //printText ("GET DESC: setup.wRequestAndType=%04X, wValue=%04X, wIndex=%04X, request.length=%d, data.length=%d", setup.wRequestAndType, setup.wValue, setup.wIndex, setup.wLength, datalen);

    if (datalen > setup.wLength) {
        datalen = setup.wLength;
    }

    size = datalen;
    if (size > EP0_SIZE) {
        size = EP0_SIZE;
    }

    //printText("1 to send: %d, sending=%d", datalen, size);
    endpoint0_transmit(data, size);
    data += size;
    datalen -= size;
    if ((datalen == 0) && (size < EP0_SIZE)) {
        //printText ("leaving 1: size: %d", size);
        return;
    }

    size = datalen;
    if (size > EP0_SIZE) {
        size = EP0_SIZE;
    }

    //printText("2 to send: %d, sending=%d", datalen, size);
    endpoint0_transmit(data, size);
    data += size;
    datalen -= size;
    if ((datalen == 0) && (size < EP0_SIZE)) {
        //printText ("leaving 2: size: %d", size);
        return;
    }

    //printText("leftover: %d", datalen);

    ep0_tx_ptr = data;
    ep0_tx_len = datalen;
}

static void usb_control(uint32_t stat)
{
    bdt_t *b;
    uint32_t pid, size;
    uint8_t *buf;
    const uint8_t *data;

    b = stat2bufferdescriptor(stat);
    pid = BDT_PID(b->desc);
    buf = b->addr;

    switch (pid) {
        case 0x0D: // Setup received from host
            setup.word1 = *(uint32_t *)(buf);
            setup.word2 = *(uint32_t *)(buf + 4);

            // give the buffer back
            b->desc = BDT_DESC(EP0_SIZE, DATA1);
            //table[index(0, RX, EVEN)].desc = BDT_DESC(EP0_SIZE, 1);
            //table[index(0, RX, ODD)].desc = BDT_DESC(EP0_SIZE, 1);

            // clear any leftover pending IN transactions
            ep0_tx_ptr = NULL;

            if (ep0_tx_data_toggle) {}
            //if (table[index(0, TX, EVEN)].desc & 0x80) {
            //serial_print("leftover tx even\n");
            //}
            //if (table[index(0, TX, ODD)].desc & 0x80) {
            //serial_print("leftover tx odd\n");
            //}
            table[index(0, TX, EVEN)].desc = 0;
            table[index(0, TX, ODD)].desc = 0;
            // first IN after Setup is always DATA1
            ep0_tx_data_toggle = 1;

            // actually "do" the setup request
            usb_setup();
            // unfreeze the USB, now that we're ready
            USB0_CTL = USB_CTL_USBENSOFEN; // clear TXSUSPENDTOKENBUSY bit
            break;

        case 0x01: // OUT transaction received from host
        case 0x02:
            // give the buffer back
            b->desc = BDT_DESC(EP0_SIZE, DATA1);
            break;

        case 0x09: // IN transaction completed to host
            // send remaining data, if any...
            data = ep0_tx_ptr;
            size = ep0_tx_len;

            if ((data) && (size > 0)) {
                if (size > EP0_SIZE) {
                    size = EP0_SIZE;
                }

                endpoint0_transmit(data, size);

                data += size;
                ep0_tx_len -= size;
                ep0_tx_ptr = (ep0_tx_len > 0 || size == EP0_SIZE) ? data : NULL;
            } else {
                // something is not quite right, act accordingly
            }

            if (setup.bRequest == 5 && setup.bmRequestType == 0) {
                setup.bRequest = 0;
                USB0_ADDR = setup.wValue;
            }

            break;
    }
    USB0_CTL = USB_CTL_USBENSOFEN; // clear TXSUSPENDTOKENBUSY bit
}

usb_packet_t *usb_rx(uint32_t endpoint)
{
    usb_packet_t *ret;

    endpoint--;

    if (endpoint >= NUM_ENDPOINTS) {
        return (NULL);
    }

    __disable_irq();
    ret = rx_first[endpoint];
    if (ret) {
        rx_first[endpoint] = ret->next;
        usb_rx_byte_count_data -= ret->len;
    }
    if (rxIsrDisabled == 0) {
        __enable_irq();
    }

    return (ret);
}

static uint32_t usb_queue_byte_count(const usb_packet_t *p)
{
    uint32_t count = 0;

    __disable_irq();
    for (; p; p = p->next) {
        count += p->len;
    }
    __enable_irq();

    return (count);
}

uint32_t usb_tx_byte_count(uint32_t endpoint)
{
    endpoint--;

    if (endpoint >= NUM_ENDPOINTS) {
        return (0);
    }
    return (usb_queue_byte_count(tx_first[endpoint]));
}

uint32_t usb_tx_packet_count(uint32_t endpoint)
{
    const usb_packet_t *p;
    uint32_t count = 0;

    endpoint--;
    if (endpoint >= NUM_ENDPOINTS) {
        return (0);
    }
    __disable_irq();
    for (p = tx_first[endpoint]; p; p = p->next) {
        count++;
    }
    __enable_irq();

    return (count);
}

void usb_rx_memory(usb_packet_t *packet)
{
    unsigned int i;
    const uint8_t *cfg;

    cfg = usb_endpoint_config_table;

    __disable_irq();

    //printText ("freeing: %d", usb_rx_byte_count_data);

    for (i = 1; i <= NUM_ENDPOINTS; i++) {
        if (*cfg++ & USB_ENDPT_EPRXEN) {
            if (table[index(i, RX, EVEN)].desc == 0) {
                table[index(i, RX, EVEN)].addr = packet->buf;
                table[index(i, RX, EVEN)].desc = BDT_DESC(64, 0);

                usb_rx_memory_needed--;

                if (rxIsrDisabled == 0) {
                    __enable_irq();
                }

                return;
            }

            if (table[index(i, RX, ODD)].desc == 0) {
                table[index(i, RX, ODD)].addr = packet->buf;
                table[index(i, RX, ODD)].desc = BDT_DESC(64, 1);
                usb_rx_memory_needed--;

                if (rxIsrDisabled == 0) {
                    __enable_irq();
                }

                return;
            }
        }
    }

    if (rxIsrDisabled == 0) {
        __enable_irq();
    }

    // we should never reach this point.  If we get here, it means
    // usb_rx_memory_needed was set greater than zero, but no memory
    // was actually needed.
    usb_rx_memory_needed = 0;
    usb_free(packet);
    return;
}

void usb_tx(uint32_t endpoint, usb_packet_t *packet)
{
    //printText ("end: %d len: %d, index: %d [%02x %d %d %d]", endpoint, packet->len, packet->index, packet->buf[0], packet->buf[1], packet->buf[2], packet->buf[3]);
    bdt_t *b = &table[index(endpoint, TX, EVEN)];
    uint8_t next;

    endpoint--;
    if (endpoint >= NUM_ENDPOINTS) {
        return;
    }
    __disable_irq();

    switch (tx_state[endpoint]) {
        case TX_STATE_BOTH_FREE_EVEN_FIRST:
            //printText ("1");
            next = TX_STATE_ODD_FREE;
            break;
        case TX_STATE_BOTH_FREE_ODD_FIRST:
            //printText ("2");
            b++;
            next = TX_STATE_EVEN_FREE;
            break;
        case TX_STATE_EVEN_FREE:
            //printText ("3");
            next = TX_STATE_NONE_FREE_ODD_FIRST;
            break;
        case TX_STATE_ODD_FREE:
            //printText ("4");
            b++;
            next = TX_STATE_NONE_FREE_EVEN_FIRST;
            break;
        default:
            if (tx_first[endpoint] == NULL) {
                //printText ("F");
                tx_first[endpoint] = packet;
            } else {
                //printText ("N");
                tx_last[endpoint]->next = packet;
            }
            tx_last[endpoint] = packet;

            __enable_irq();

            return;
    }
    //printText ("X");
    tx_state[endpoint] = next;
    b->addr = packet->buf;
    b->desc = BDT_DESC(packet->len, ((uint32_t)b & 8) ? DATA1 : DATA0);

    __enable_irq();
}

void _reboot_Teensyduino_(void)
{
    // TODO: initialize R0 with a code....
    __asm__ volatile("bkpt");
}

void usb_isr(void)
{
    uint8_t status;
    uint8_t stat;

restart:
    status = USB0_ISTAT;

    /*
	 * Start of the frame (SOF)
	 */
    if ((status & USB_ISTAT_SOFTOK /* 04 */)) {
        /*
		 * TODO: There should be a check that SOF arrives every 0.5 - 1.5 mSec
		 */

        /*
		 * Flush midi output on each Start of frame
		 */
        if (usb_configuration) {
            usb_midi_flush_output();
        }

        USB0_ISTAT = USB_ISTAT_SOFTOK;
    }

    /*
	 * Token exchange (sending and receiving)
	 */
    if ((status & USB_ISTAT_TOKDNE /* 08 */)) {
        stat = USB0_STAT;
        uint8_t endpoint = stat >> 4;
        ;

        /*
		 * Control transfer
		 */
        if (endpoint == 0) {
            usb_control(stat);
        }

        /*
		 * Interface data transfer
		 */
        else {
            //printText ("isr: ep: %d", endpoint - 1);

            bdt_t *b = stat2bufferdescriptor(stat);
            usb_packet_t *packet = (usb_packet_t *)((uint8_t *)(b->addr) - 8);

            endpoint--; // endpoint is index to zero-based arrays

            /*
			 * Send (TX) data to the host
			 */
            if (stat & 0x08) // transmit
            {
                usb_free_tx(packet);
                packet = tx_first[endpoint];
                if (packet) {
                    //printText ("Y");
                    //serial_print("tx packet\n");
                    tx_first[endpoint] = packet->next;
                    b->addr = packet->buf;
                    switch (tx_state[endpoint]) {
                        case TX_STATE_BOTH_FREE_EVEN_FIRST:
                            tx_state[endpoint] = TX_STATE_ODD_FREE;
                            break;
                        case TX_STATE_BOTH_FREE_ODD_FIRST:
                            tx_state[endpoint] = TX_STATE_EVEN_FREE;
                            break;
                        case TX_STATE_EVEN_FREE:
                            tx_state[endpoint] = TX_STATE_NONE_FREE_ODD_FIRST;
                            break;
                        case TX_STATE_ODD_FREE:
                            tx_state[endpoint] = TX_STATE_NONE_FREE_EVEN_FIRST;
                            break;
                        default:
                            break;
                    }
                    b->desc = BDT_DESC(packet->len,
                                       ((uint32_t)b & 8) ? DATA1 : DATA0);
                } else {
                    //serial_print("tx no packet\n");
                    //printText ("N");
                    switch (tx_state[endpoint]) {
                        case TX_STATE_BOTH_FREE_EVEN_FIRST:
                        case TX_STATE_BOTH_FREE_ODD_FIRST:
                            break;
                        case TX_STATE_EVEN_FREE:
                            tx_state[endpoint] = TX_STATE_BOTH_FREE_EVEN_FIRST;
                            break;
                        case TX_STATE_ODD_FREE:
                            tx_state[endpoint] = TX_STATE_BOTH_FREE_ODD_FIRST;
                            break;
                        default:
                            tx_state[endpoint] = ((uint32_t)b & 8)
                                                     ? TX_STATE_ODD_FREE
                                                     : TX_STATE_EVEN_FREE;
                            break;
                    }
                }
            }

            /*
			 * Receive (RX) data from the host
			 */
            else // receive
            {
                //printText ("rec: %d", endpoint);
                packet->len = b->desc >> 16;

                if (packet->len > 0) {
                    packet->index = 0;
                    packet->next = NULL;
                    if (rx_first[endpoint] == NULL) {
                        rx_first[endpoint] = packet;
                    } else {
                        rx_last[endpoint]->next = packet;
                    }
                    rx_last[endpoint] = packet;

                    usb_rx_byte_count_data += packet->len;

                    packet = usb_malloc();

                    if (packet) {
                        b->addr = packet->buf;
                        b->desc =
                            BDT_DESC(64, ((uint32_t)b & 8) ? DATA1 : DATA0);
                    } else {
                        b->desc = 0;
                        usb_rx_memory_needed++;
                    }
                } else {
                    b->desc = BDT_DESC(64, ((uint32_t)b & 8) ? DATA1 : DATA0);
                }
            }
        }
        USB0_ISTAT = USB_ISTAT_TOKDNE;
        goto restart;
    }

    /*
	 * USB Reset request
	 */
    if (status & USB_ISTAT_USBRST /* 01 */) {
        // initialize BDT toggle bits
        USB0_CTL = USB_CTL_ODDRST;
        ep0_tx_bdt_bank = 0;

        // set up buffers to receive Setup and OUT packets
        table[index(0, RX, EVEN)].desc = BDT_DESC(EP0_SIZE, 0);
        table[index(0, RX, EVEN)].addr = ep0_rx0_buf;
        table[index(0, RX, ODD)].desc = BDT_DESC(EP0_SIZE, 0);
        table[index(0, RX, ODD)].addr = ep0_rx1_buf;
        table[index(0, TX, EVEN)].desc = 0;
        table[index(0, TX, ODD)].desc = 0;

        // activate endpoint 0
        USB0_ENDPT0 = USB_ENDPT_EPRXEN | USB_ENDPT_EPTXEN | USB_ENDPT_EPHSHK;

        // clear all ending interrupts
        USB0_ERRSTAT = 0xFF;
        USB0_ISTAT = 0xFF;

        // set the address to zero during enumeration
        USB0_ADDR = 0;

        // enable other interrupts
        USB0_ERREN = 0xFF;
        USB0_INTEN = USB_INTEN_TOKDNEEN | USB_INTEN_SOFTOKEN | USB_INTEN_STALLEN
                     | USB_INTEN_ERROREN | USB_INTEN_USBRSTEN
                     | USB_INTEN_SLEEPEN;

        // is this necessary?
        USB0_CTL = USB_CTL_USBENSOFEN;
        return;
    }

    /*
	 * USB Stalled
	 */
    if ((status & USB_ISTAT_STALL /* 80 */)) {
        USB0_ENDPT0 = USB_ENDPT_EPRXEN | USB_ENDPT_EPTXEN | USB_ENDPT_EPHSHK;
        USB0_ISTAT = USB_ISTAT_STALL;
    }

    /*
	 * USB error
	 */
    if ((status & USB_ISTAT_ERROR /* 02 */)) {
        uint8_t err = USB0_ERRSTAT;
        USB0_ERRSTAT = err;
        //serial_print("err:");
        //serial_phex(err);
        //serial_print("\n");
        USB0_ISTAT = USB_ISTAT_ERROR;
    }

    /*
	 * USB sleep
	 */
    if ((status & USB_ISTAT_SLEEP /* 10 */)) {
        //serial_print("sleep\n");
        USB0_ISTAT = USB_ISTAT_SLEEP;
    }
}

void usb_init(uint8_t keyboardEnabled,
              uint8_t mouseEnabled,
              uint8_t keymediaEnabled)
{
    uint16_t descriptorPosition = MINIMUM_CONFIG_SIZE;
    uint8_t interfaceNumber = 2;

    if (keyboardEnabled == 1) {
        KEYBOARD_INTERFACE = interfaceNumber;
        KEYBOARD_ENDPOINT = interfaceNumber + 1;
        config_descriptor_keyboard[2] = KEYBOARD_INTERFACE;
        config_descriptor_keyboard[20] =
            KEYBOARD_ENDPOINT | 0x80; // bEndpointAddress
        memcpy(config_descriptor + descriptorPosition,
               config_descriptor_keyboard,
               KEYBOARD_INTERFACE_DESC_SIZE);
        usb_descriptor_list[2].wIndex = KEYBOARD_INTERFACE;
        usb_descriptor_list[3].wIndex = KEYBOARD_INTERFACE;
        usb_descriptor_list[3].addr =
            config_descriptor + descriptorPosition + 9;
        usb_endpoint_config_table[interfaceNumber] = ENDPOINT_TRANSMIT_ONLY;
        descriptorPosition += KEYBOARD_INTERFACE_DESC_SIZE;
        interfaceNumber++;
    }

    if (mouseEnabled == 1) {
        MOUSE_INTERFACE = interfaceNumber;
        MOUSE_ENDPOINT = interfaceNumber + 1;
        config_descriptor_mouse[2] = MOUSE_INTERFACE;
        config_descriptor_mouse[20] = MOUSE_ENDPOINT | 0x80;
        memcpy(config_descriptor + descriptorPosition,
               config_descriptor_mouse,
               MOUSE_INTERFACE_DESC_SIZE);
        usb_descriptor_list[4].wIndex = MOUSE_INTERFACE;
        usb_descriptor_list[5].wIndex = MOUSE_INTERFACE;
        usb_descriptor_list[5].addr =
            config_descriptor + descriptorPosition + 9;
        usb_endpoint_config_table[interfaceNumber] = ENDPOINT_TRANSMIT_ONLY;
        descriptorPosition += MOUSE_INTERFACE_DESC_SIZE;
        interfaceNumber++;
    }

    if (keymediaEnabled == 1) {
        KEYMEDIA_INTERFACE = interfaceNumber;
        KEYMEDIA_ENDPOINT = interfaceNumber + 1;
        config_descriptor_keymedia[2] = KEYMEDIA_INTERFACE;
        config_descriptor_keymedia[20] =
            KEYMEDIA_ENDPOINT | 0x80; // bEndpointAddress
        memcpy(config_descriptor + descriptorPosition,
               config_descriptor_keymedia,
               KEYMEDIA_INTERFACE_DESC_SIZE);
        usb_descriptor_list[6].wIndex = KEYMEDIA_INTERFACE;
        usb_descriptor_list[7].wIndex = KEYMEDIA_INTERFACE;
        usb_descriptor_list[7].addr =
            config_descriptor + descriptorPosition + 9;
        usb_endpoint_config_table[interfaceNumber] = ENDPOINT_TRANSMIT_ONLY;
        descriptorPosition += KEYMEDIA_INTERFACE_DESC_SIZE;
        interfaceNumber++;
    }

    NUM_INTERFACE = interfaceNumber;
    NUM_ENDPOINTS = interfaceNumber;

    config_descriptor[2] = LSB(descriptorPosition);
    config_descriptor[3] = MSB(descriptorPosition);
    config_descriptor[4] = NUM_INTERFACE;
    usb_descriptor_list[1].length = descriptorPosition;

    int i;

    rxIsrDisabled = 0;

    for (i = 0; i <= MAX_NUM_ENDPOINTS * 4; i++) {
        table[i].desc = 0;
        table[i].addr = 0;
    }

    // this basically follows the flowchart in the Kinetis
    // Quick Reference User Guide, Rev. 1, 03/2012, page 141

    // assume 48 MHz clock already running
    // SIM - enable clock
    SIM_SCGC4 |= SIM_SCGC4_USBOTG;
    MPU_RGDAAC0 |= 0x03000000;

    // if using IRC48M, turn on the USB clock recovery hardware
    USB0_CLK_RECOVER_IRC_EN =
        USB_CLK_RECOVER_IRC_EN_IRC_EN | USB_CLK_RECOVER_IRC_EN_REG_EN;
    USB0_CLK_RECOVER_CTRL = USB_CLK_RECOVER_CTRL_CLOCK_RECOVER_EN
                            | USB_CLK_RECOVER_CTRL_RESTART_IFRTRIM_EN;

    // reset USB module
    //USB0_USBTRC0 = USB_USBTRC_USBRESET;
    //while ((USB0_USBTRC0 & USB_USBTRC_USBRESET) != 0) ; // wait for reset to end

    // set desc table base addr
    USB0_BDTPAGE1 = ((uint32_t)table) >> 8;
    USB0_BDTPAGE2 = ((uint32_t)table) >> 16;
    USB0_BDTPAGE3 = ((uint32_t)table) >> 24;

    // clear all ISR flags
    USB0_ISTAT = 0xFF;
    USB0_ERRSTAT = 0xFF;
    USB0_OTGISTAT = 0xFF;

    //USB0_USBTRC0 |= 0x40; // undocumented bit

    // enable USB
    USB0_CTL = USB_CTL_USBENSOFEN;
    USB0_USBCTRL = 0;

    // enable reset interrupt
    USB0_INTEN = USB_INTEN_USBRSTEN;

    // enable interrupt in NVIC...
    NVIC_SET_PRIORITY(IRQ_USBOTG, 112);
    NVIC_ENABLE_IRQ(IRQ_USBOTG);

    // enable d+ pullup
    USB0_CONTROL = USB_CONTROL_DPPULLUPNONOTG;
}

void usb_stop(void)
{
    USB0_USBTRC0 = USB_USBTRC_USBRESET;
}
