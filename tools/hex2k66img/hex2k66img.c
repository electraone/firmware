/*
 * The file converts the HEX file to NXP MK66FX1 specific binary image.
 *
 * Extracted from Teensy Loader sources and based on:
 * ihex.c, at http://www.pjrc.com/tech/8051/pm2_docs/intel-hex.html
 */

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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <string.h>

// default settings (reflects NXP MK66FX1 settings)
#define CODE_SIZE  1048576
#define BLOCK_SIZE 1024


// Intel Hex file functions
int read_intel_hex(const char *filename);
int ihex_bytes_within_range(int begin, int end);
void ihex_get_data(int addr, int len, unsigned char *bytes);
int memory_is_blank(int addr, int block_size);


/*
 * Main
 */
int main(int argc, char **argv) {
  unsigned char buf[2048];
  int addr;
  int write_size = BLOCK_SIZE + 2;
  int first_block = 1;
  unsigned int fileLength = 0;
  const char *inputFilename = NULL;
  const char *outputFilename = NULL;
  FILE *outputFile = NULL;

  if (argc == 3) {
    inputFilename = argv[1];
    outputFilename = argv[2];
  } else {
    fprintf(
        stderr,
        "hex2k66img v1.0\n\n"
        "Converts a Hex file to binary to binary image for NXP MK66FX1 mcu.\n\n"
        "Usage: hex2k66img input-hex-file output-img-file\n\n");
    exit(1);
  }

  if (access(inputFilename, F_OK) != 0) {
    fprintf(stderr, "Hex file does not exist: \"%s\"", inputFilename);
    exit(1);
  }

  // read HEX file to memory
  fileLength = read_intel_hex(inputFilename);

  if (fileLength < 0) {
    fprintf(stderr, "Not a valid Hex file: \"%s\"", inputFilename);
    exit(1);
  }

  // Open the output Img file for writing
  outputFile = fopen(outputFilename, "w");

  if (outputFile == NULL) {
    fprintf(stderr, "Failed to open the Img file for writing: \"%s\"\n",
	      outputFilename);
    exit(1);
  }

  // Generate the Img file
  for (addr = 0; addr < CODE_SIZE; addr += BLOCK_SIZE) {
    if (!first_block && !ihex_bytes_within_range(addr, addr + BLOCK_SIZE - 1)) {
      // don't waste time on blocks that are unused,
      // but always do the first one to erase the chip
      continue;
    }

    if (!first_block && memory_is_blank(addr, BLOCK_SIZE)) {
      continue;
    }

    buf[0] = addr & 255;
    buf[1] = (addr >> 8) & 255;
    buf[2] = (addr >> 16) & 255;
    memset(buf + 3, 0, 61);
    ihex_get_data(addr, BLOCK_SIZE, buf + 64);
    write_size = BLOCK_SIZE + 64;

    fwrite(buf, sizeof(unsigned char), write_size, outputFile);

    first_block = 0;
  }

  fclose(outputFile);

  return 0;
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
static int end_record_seen = 0;
static int byte_count;
static unsigned int extended_addr = 0;
static int parse_hex_line(char *line);

int read_intel_hex(const char *filename) {
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
    // printf("Unable to read file %s\n", filename);
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

int parse_hex_line(char *line) {
  int addr, code, num;
  int sum, len, cksum, i;
  char *ptr;

  num = 0;
  if (line[0] != ':') return 0;
  if (strlen(line) < 11) return 0;
  ptr = line + 1;

  if (!sscanf(ptr, "%02x", &len)) return 0;
  ptr += 2;

  if ((int)strlen(line) < (11 + (len * 2))) return 0;
  if (!sscanf(ptr, "%04x", &addr)) return 0;
  ptr += 4;

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
    }

    if (code == 4 && len == 2) {
      if (!sscanf(ptr, "%04x", &i)) return 1;
      ptr += 4;
      sum += ((i >> 8) & 255) + (i & 255);
      if (!sscanf(ptr, "%02x", &cksum)) return 1;
      if (((sum & 255) + (cksum & 255)) & 255) return 1;
      extended_addr = i << 16;
    }
    return 1;
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

int ihex_bytes_within_range(int begin, int end) {
  int i;

  if (begin < 0 || begin >= MAX_MEMORY_SIZE || end < 0 ||
      end >= MAX_MEMORY_SIZE) {
    return 0;
  }

  for (i = begin; i <= end; i++) {
    if (firmware_mask[i]) return 1;
  }

  return 0;
}

void ihex_get_data(int addr, int len, unsigned char *bytes) {
  int i;

  if (addr < 0 || len < 0 || addr + len >= MAX_MEMORY_SIZE) {
    for (i = 0; i < len; i++) {
      bytes[i] = 255;
    }
    return;
  }

  for (i = 0; i < len; i++) {
    if (firmware_mask[addr]) {
      bytes[i] = firmware_image[addr];
    } else {
      bytes[i] = 255;
    }
    addr++;
  }
}

int memory_is_blank(int addr, int block_size) {
  if (addr < 0 || addr > MAX_MEMORY_SIZE) return 1;

  while (block_size && addr < MAX_MEMORY_SIZE) {
    if (firmware_mask[addr] && firmware_image[addr] != 255) return 0;
    addr++;
    block_size--;
  }

  return 1;
}
