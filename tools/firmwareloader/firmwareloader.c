/* Teensy Loader, Command Line Interface
 * Copyright 2008-2016, PJRC.COM, LLC
 * of the GNU General Public License as published by the Free Software
 * Foundation, version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/
 */

/* Want to incorporate this code into a proprietary application??
 * Just email paul@pjrc.com to ask.  Usually it's not a problem,
 * but you do need to ask to use this code in any way other than
 * those permitted by the GNU General Public License, version 3  */

/* For non-root permissions on ubuntu or similar udev-based linux
 * http://www.pjrc.com/teensy/49-teensy.rules
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>

/*
 * Low-level MacOS X USB functions
 */
#include <IOKit/IOKitLib.h>
#include <IOKit/hid/IOHIDLib.h>
#include <IOKit/hid/IOHIDDevice.h>

struct usb_list_struct {
	IOHIDDeviceRef ref;
	int pid;
	int vid;
	struct usb_list_struct *next;
};

static struct usb_list_struct *usb_list=NULL;
static IOHIDManagerRef hid_manager=NULL;
static IOHIDDeviceRef iokit_teensy_reference = NULL;

void attach_callback(void *context, IOReturn r, void *hid_mgr, IOHIDDeviceRef dev);
void detach_callback(void *context, IOReturn r, void *hid_mgr, IOHIDDeviceRef dev);
void init_hid_manager(void);
static void do_run_loop(void);
IOHIDDeviceRef open_usb_device(int vid, int pid);
void close_usb_device(IOHIDDeviceRef dev);


/*
 * Teensy specific USB access functions
 */
int teensy_open(void);
int teensy_write(void *buf, int len, double timeout);
void teensy_close(void);
int hard_reboot(void);
void boot(unsigned char *buf, int write_size);


/*
 * Intel Hex file functions
 */
int read_intel_hex(const char *filename);
int ihex_bytes_within_range(int begin, int end);
void ihex_get_data(int addr, int len, unsigned char *bytes);
int memory_is_blank(int addr, int block_size);


/*
 * Misc stuff
 */
void die(const char *str, ...);


/*
 * default settings
 */
int wait_for_device_to_appear = 1;
int hard_reboot_device = 1;
int code_size = 1048576;                   // reflects MK66FX1 settings
int block_size = 1024;										 // reflects MK66FX1 settings
const char *filename = "";


/*
 * Main
 */
int main(int argc, char **argv)
{
	unsigned char buf[2048];
	int addr;
	int r;
	int write_size = block_size + 2;
	int first_block = 1;
	int waited = 0;
	int fileLength = 0;

  filename = argv[1];

	printf("Electra firmware loader, v1.0\n");


  /*
	 * read HEX file to memory
	 */
	fileLength = read_intel_hex(filename);

  if (fileLength < 0) {
		fprintf(stderr, "cannot read firmware file \"%s\"\n", filename);
		exit(1);
	}


	// open the USB device
	while (1) {
		if (teensy_open()) {
			break;
		}

		if (hard_reboot_device == 1) {
			printf("Programming mode entered\n");
			hard_reboot_device = 0; // only hard reboot once
			wait_for_device_to_appear = 1;
		}

		if (!wait_for_device_to_appear) {
			fprintf(stderr, "Electra not connected to USB\n");
			exit(1);
		}

		if (!waited) {
			printf("Waiting for Electra to appear\n");
			waited = 1;
		}

		usleep(250000);
	}


	/*
	 * Write the HEX file to the teensy
	 */
	printf("Uploading firmware: ");
	fflush(stdout);

	for (addr = 0; addr < code_size; addr += block_size) {

		if (!first_block && !ihex_bytes_within_range(addr, addr + block_size - 1)) {
			// don't waste time on blocks that are unused,
			// but always do the first one to erase the chip
			continue;
		}

		if (!first_block && memory_is_blank(addr, block_size)) {
			continue;
		}

		printf(".");
    fflush(stdout);

		buf[0] = addr & 255;
		buf[1] = (addr >> 8) & 255;
		buf[2] = (addr >> 16) & 255;
		memset(buf + 3, 0, 61);
		ihex_get_data(addr, block_size, buf + 64);
		write_size = block_size + 64;

		r = teensy_write(buf, write_size, first_block ? 5.0 : 0.5);

		if (!r) {
			fprintf(stderr, "error writing to Teensy\n");
			exit(1);
		}

		first_block = 0;
	}

	printf("\n");
  boot(buf, write_size);
	teensy_close();
	
	return 0;
}

int teensy_open(void)
{
	teensy_close();
	iokit_teensy_reference = open_usb_device(0x16C0, 0x0478);
	if (iokit_teensy_reference) return 1;
	return 0;
}

int teensy_write(void *buf, int len, double timeout)
{
	IOReturn ret;

	if (!iokit_teensy_reference) {
		return 0;
	}

	double start = CFAbsoluteTimeGetCurrent();

	while (CFAbsoluteTimeGetCurrent() - timeout < start) {
		ret = IOHIDDeviceSetReport(iokit_teensy_reference, kIOHIDReportTypeOutput, 0, buf, len);

		if (ret == kIOReturnSuccess) {
			return 1;
		}

		usleep(10000);
	}

	return 0;
}

void teensy_close(void)
{
	if (!iokit_teensy_reference) {
		return;
	}

	close_usb_device(iokit_teensy_reference);
	iokit_teensy_reference = NULL;
}

int hard_reboot(void)
{
	IOHIDDeviceRef rebootor;
	IOReturn ret;

	rebootor = open_usb_device(0x16C0, 0x0477);

	if (!rebootor) {
		return 0;
	}

	ret = IOHIDDeviceSetReport(rebootor, kIOHIDReportTypeOutput, 0, (uint8_t *)("reboot"), 6);
	close_usb_device(rebootor);

	if (ret == kIOReturnSuccess) {
		return 1;
	}

	return 0;
}

void boot(unsigned char *buf, int write_size)
{
	printf("Restarting Electra\n");
	memset(buf, 0, write_size);
	buf[0] = 0xFF;
	buf[1] = 0xFF;
	buf[2] = 0xFF;
	teensy_write(buf, write_size, 0.5);
}


/****************************************************************/
/*                                                              */
/*                     Read Intel Hex File                      */
/*                                                              */
/****************************************************************/

// the maximum flash image size we can support
// chips with larger memory may be used, but only this
// much intel-hex data can be loaded into memory!
#define MAX_MEMORY_SIZE 0x100000

static unsigned char firmware_image[MAX_MEMORY_SIZE];
static unsigned char firmware_mask[MAX_MEMORY_SIZE];
static int end_record_seen=0;
static int byte_count;
static unsigned int extended_addr = 0;
static int parse_hex_line(char *line);

int read_intel_hex(const char *filename)
{
	FILE *fp;
	int i;
	int lineno = 0;
	char buf[1024];

	byte_count = 0;
	end_record_seen = 0;

  /*
	 * create image filled with 0xFF
	 */
	for (i = 0; i < MAX_MEMORY_SIZE; i++) {
		firmware_image[i] = 0xFF;
		firmware_mask[i] = 0;
	}
	extended_addr = 0;

	fp = fopen(filename, "r");
	if (fp == NULL) {
		//printf("Unable to read file %s\n", filename);
		return -1;
	}

	while (!feof(fp)) {
		*buf = '\0';

		if (!fgets(buf, sizeof(buf), fp)) {
			break;
		}

		lineno++;

		if (*buf) {
			if (parse_hex_line(buf) == 0) {
				printf("Warning, HEX parse error line %d\n", lineno);
				return -2;
			}
		}
		if (end_record_seen) {
			break;
		}

		if (feof(stdin)) {
			break;
		}
	}
	fclose(fp);

	return byte_count;
}


/* from ihex.c, at http://www.pjrc.com/tech/8051/pm2_docs/intel-hex.html */

/* parses a line of intel hex code, stores the data in bytes[] */
/* and the beginning address in addr, and returns a 1 if the */
/* line was valid, or a 0 if an error occured.  The variable */
/* num gets the number of bytes that were stored into bytes[] */


int
parse_hex_line(char *line)
{
	int addr, code, num;
  int sum, len, cksum, i;
  char *ptr;

  num = 0;
  if (line[0] != ':') return 0;
  if (strlen(line) < 11) return 0;
  ptr = line + 1;

  if (!sscanf(ptr, "%02x", &len)) return 0;
  ptr += 2;

  if ((int)strlen(line) < (11 + (len * 2)) ) return 0;
  if (!sscanf(ptr, "%04x", &addr)) return 0;
  ptr += 4;
  /* printf("Line: length=%d Addr=%d\n", len, addr); */

  if (!sscanf(ptr, "%02x", &code)) return 0;

	if (addr + extended_addr + len >= MAX_MEMORY_SIZE) return 0;
  ptr += 2;
  sum = (len & 255) + ((addr >> 8) & 255) + (addr & 255) + (code & 255);

	if (code != 0) {
		if (code == 1) {
			end_record_seen = 1;
			return 1;
		}
		if (code == 2 && len == 2) {
			if (!sscanf(ptr, "%04x", &i)) return 1;
			ptr += 4;
			sum += ((i >> 8) & 255) + (i & 255);
        		if (!sscanf(ptr, "%02x", &cksum)) return 1;
			if (((sum & 255) + (cksum & 255)) & 255) return 1;
			extended_addr = i << 4;
			//printf("ext addr = %05X\n", extended_addr);
		}
		if (code == 4 && len == 2) {
			if (!sscanf(ptr, "%04x", &i)) return 1;
			ptr += 4;
			sum += ((i >> 8) & 255) + (i & 255);
        		if (!sscanf(ptr, "%02x", &cksum)) return 1;
			if (((sum & 255) + (cksum & 255)) & 255) return 1;
			extended_addr = i << 16;
			//printf("ext addr = %08X\n", extended_addr);
		}
		return 1;	// non-data line
	}
	byte_count += len;

  while (num != len) {
    if (sscanf(ptr, "%02x", &i) != 1) return 0;

		i &= 255;
		firmware_image[addr + extended_addr + num] = i;
		firmware_mask[addr + extended_addr + num] = 1;
    ptr += 2;
    sum += i;
    (num)++;
    if (num >= 256) return 0;
  }
  if (!sscanf(ptr, "%02x", &cksum)) return 0;
  if (((sum & 255) + (cksum & 255)) & 255) return 0; /* checksum error */
  return 1;
}

int ihex_bytes_within_range(int begin, int end)
{
	int i;

	if (begin < 0 || begin >= MAX_MEMORY_SIZE ||
	   end < 0 || end >= MAX_MEMORY_SIZE) {
		return 0;
	}

	for (i=begin; i<=end; i++) {
		if (firmware_mask[i]) return 1;
	}

	return 0;
}

void ihex_get_data(int addr, int len, unsigned char *bytes)
{
	int i;

	if (addr < 0 || len < 0 || addr + len >= MAX_MEMORY_SIZE) {
		for (i=0; i<len; i++) {
			bytes[i] = 255;
		}
		return;
	}
	for (i=0; i<len; i++) {
		if (firmware_mask[addr]) {
			bytes[i] = firmware_image[addr];
		}
		else {
			bytes[i] = 255;
		}
		addr++;
	}
}

int memory_is_blank(int addr, int block_size)
{
	if (addr < 0 || addr > MAX_MEMORY_SIZE) return 1;

	while (block_size && addr < MAX_MEMORY_SIZE) {
		if (firmware_mask[addr] && firmware_image[addr] != 255) return 0;
		addr++;
		block_size--;
	}
	return 1;
}



/*
 * Low-Level MacOS X USB handling functions
 */
void attach_callback(void *context, IOReturn r, void *hid_mgr, IOHIDDeviceRef dev)
{
 	CFTypeRef type;
 	struct usb_list_struct *n, *p;
 	int32_t pid, vid;

 	if (!dev) return;
 	type = IOHIDDeviceGetProperty(dev, CFSTR(kIOHIDVendorIDKey));
 	if (!type || CFGetTypeID(type) != CFNumberGetTypeID()) return;
 	if (!CFNumberGetValue((CFNumberRef)type, kCFNumberSInt32Type, &vid)) return;
 	type = IOHIDDeviceGetProperty(dev, CFSTR(kIOHIDProductIDKey));
 	if (!type || CFGetTypeID(type) != CFNumberGetTypeID()) return;
 	if (!CFNumberGetValue((CFNumberRef)type, kCFNumberSInt32Type, &pid)) return;
 	n = (struct usb_list_struct *)malloc(sizeof(struct usb_list_struct));
 	if (!n) return;
 	n->ref = dev;
 	n->vid = vid;
 	n->pid = pid;
 	n->next = NULL;
 	if (usb_list == NULL) {
 		usb_list = n;
 	} else {
 		for (p = usb_list; p->next; p = p->next) ;
 		p->next = n;
 	}
}

void detach_callback(void *context, IOReturn r, void *hid_mgr, IOHIDDeviceRef dev)
{
 	struct usb_list_struct *p, *tmp, *prev=NULL;

 	p = usb_list;
 	while (p) {
 		if (p->ref == dev) {
 			if (prev) {
 				prev->next = p->next;
 			} else {
 				usb_list = p->next;
 			}
 			tmp = p;
 			p = p->next;
 			free(tmp);
 		} else {
 			prev = p;
 			p = p->next;
 		}
 	}
}

void init_hid_manager(void)
{
 	CFMutableDictionaryRef dict;
 	IOReturn ret;

 	if (hid_manager) return;
 	hid_manager = IOHIDManagerCreate(kCFAllocatorDefault, kIOHIDOptionsTypeNone);
 	if (hid_manager == NULL || CFGetTypeID(hid_manager) != IOHIDManagerGetTypeID()) {
 		if (hid_manager) CFRelease(hid_manager);
 		printf("no HID Manager - maybe this is a pre-Leopard (10.5) system?\n");
 		return;
 	}
 	dict = CFDictionaryCreateMutable(kCFAllocatorDefault, 0,
 		&kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
 	if (!dict) return;
 	IOHIDManagerSetDeviceMatching(hid_manager, dict);
 	CFRelease(dict);
 	IOHIDManagerScheduleWithRunLoop(hid_manager, CFRunLoopGetCurrent(), kCFRunLoopDefaultMode);
 	IOHIDManagerRegisterDeviceMatchingCallback(hid_manager, attach_callback, NULL);
 	IOHIDManagerRegisterDeviceRemovalCallback(hid_manager, detach_callback, NULL);
 	ret = IOHIDManagerOpen(hid_manager, kIOHIDOptionsTypeNone);
 	if (ret != kIOReturnSuccess) {
 		IOHIDManagerUnscheduleFromRunLoop(hid_manager,
 			CFRunLoopGetCurrent(), kCFRunLoopDefaultMode);
 		CFRelease(hid_manager);
 		printf("Error opening HID Manager\n");
 	}
}

static void do_run_loop(void)
{
 	while (CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0, true) == kCFRunLoopRunHandledSource) ;
}

IOHIDDeviceRef open_usb_device(int vid, int pid)
{
 	struct usb_list_struct *p;
 	IOReturn ret;

 	init_hid_manager();
 	do_run_loop();
 	for (p = usb_list; p; p = p->next) {
 		if (p->vid == vid && p->pid == pid) {
 			ret = IOHIDDeviceOpen(p->ref, kIOHIDOptionsTypeNone);
 			if (ret == kIOReturnSuccess) return p->ref;
 		}
 	}
 	return NULL;
}

void close_usb_device(IOHIDDeviceRef dev)
{
 	struct usb_list_struct *p;

 	do_run_loop();
 	for (p = usb_list; p; p = p->next) {
 		if (p->ref == dev) {
 			IOHIDDeviceClose(dev, kIOHIDOptionsTypeNone);
 			return;
 		}
 	}
}
