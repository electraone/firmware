/*
 * A set of helper functions
 */
#include "helpers.h"
#include <cstdio>
#include <cstdarg>
#include <cstring>
#include "Arduino.h"
#include "settings.h"

volatile bool loggerEnabled;

#define LOG_MESSAGE_MAX_SIZE 200 // Max size of the log message

static const int col_pos[6] = { 0, 170, 340, 510, 680, 850 };
static const int row_pos[6] = { 40, 128, 216, 304, 392, 480 };

const char *noteNames[] = {
    "c-1",  "c#-1", "d-1", "d#-1", "e-1", "f-1", "f#-1", "g-1", "g#-1", "a-1",
    "a#-1", "b-1",  "c0",  "c#0",  "d0",  "d#0", "e0",   "f0",  "f#0",  "g0",
    "g#0",  "a0",   "a#0", "b0",   "c1",  "c#1", "d1",   "d#1", "e1",   "f1",
    "f#1",  "g1",   "g#1", "a1",   "a#1", "b1",  "c2",   "c#2", "d2",   "d#2",
    "e2",   "f2",   "f#2", "g2",   "g#2", "a2",  "a#2",  "b2",  "c3",   "c#3",
    "d3",   "d#3",  "e3",  "f3",   "f#3", "g3",  "g#3",  "a3",  "a#3",  "b3",
    "c4",   "c#4",  "d4",  "d#4",  "e4",  "f4",  "f#4",  "g4",  "g#4",  "a4",
    "a#4",  "b4",   "c5",  "c#5",  "d5",  "d#5", "e5",   "f5",  "f#5",  "g5",
    "g#5",  "a5",   "a#5", "b5",   "c6",  "c#6", "d6",   "d#6", "e6",   "f6",
    "f#6",  "g6",   "g#6", "a6",   "a#6", "b6",  "c7",   "c#7", "d7",   "d#7",
    "e7",   "f7",   "f#7", "g7",   "g#7", "a7",  "a#7",  "b7",  "c8",   "c#8",
    "d8",   "d#8",  "e8",  "f8",   "f#8", "g8",  "g#8",  "a8",  "a#8",  "b8",
    "c9",   "c#9",  "d9",  "d#9",  "e9",  "f9",  "f#9",  "g9",
};

const char *getNoteName(uint8_t noteNr)
{
    return (noteNames[noteNr]);
}

void logMessage(const char *format, ...)
{
    if (!loggerEnabled) {
        return;
    }

    char buf[LOG_MESSAGE_MAX_SIZE];
    va_list ap;

    va_start(ap, format);
    vsnprintf(buf + 5, sizeof(buf) - 5, format, ap);
    va_end(ap);

    buf[0] = 0x00;
    buf[1] = 0x21;
    buf[2] = 0x45;
    buf[3] = 0x7f;
    buf[4] = 0x00;

    for (uint16_t i = 5; (i < (sizeof(buf) - 5)) && (buf[i] != '\0'); i++) {
        if ((buf[i] < 32) || (buf[i] > 126)) {
            buf[i] = '#';
        }
    }

    usbMIDI.sendSysEx(strlen(buf + 5) + 5, (const uint8_t *)buf, false, 2);
}

int approxRollingAverage(int avg, int input)
{
    if (avg == 0) {
        avg = input;
    } else {
        int N = 10;
        avg -= avg / N;
        avg += input / N;

        avg = (avg + input) / 2;
    }
    return (avg);
}

void copyString(char *dest, const char *src, uint16_t maxLength)
{
    if (src) {
        strncpy(dest, src, maxLength);
        dest[maxLength] = '\0';
    } else {
        dest[0] = '\0';
    }
}

void toUpperCase(char *str)
{
    char *pStr = str;

    if (pStr) {
        while (*pStr) {
            *pStr = toupper(*pStr);
            pStr++;
        }
    }
}

uint16_t translateColor(uint8_t colorId)
{
    switch (colorId) {
        case 1:
            return (COLOR_TILE_NAME_1);

        case 2:
            return (COLOR_TILE_NAME_2);

        case 3:
            return (COLOR_TILE_NAME_3);

        case 4:
            return (COLOR_TILE_NAME_4);

        case 5:
            return (COLOR_TILE_NAME_5);

        case 6:
            return (COLOR_TILE_NAME_6);

        case 7:
            return (COLOR_TILE_NAME_7);

        case 8:
            return (COLOR_TILE_NAME_8);

        case 9:
            return (COLOR_TILE_NAME_9);
    }
    return (COLOR_TILE_NAME_1);
}

uint8_t translateColorToId(uint16_t colorRGB)
{
    switch (colorRGB) {
        case COLOR_TILE_NAME_1:
            return (1);

        case COLOR_TILE_NAME_2:
            return (2);

        case COLOR_TILE_NAME_3:
            return (3);

        case COLOR_TILE_NAME_4:
            return (4);

        case COLOR_TILE_NAME_5:
            return (5);

        case COLOR_TILE_NAME_6:
            return (6);

        case COLOR_TILE_NAME_7:
            return (7);

        case COLOR_TILE_NAME_8:
            return (8);

        case COLOR_TILE_NAME_9:
            return (9);
    }
    return (1);
}

uint16_t getBackgroudColor(uint16_t fgColor)
{
    switch (fgColor) {
        case 0xFFFF:
            return (0x3186);

        case 0xEAC9:
            return (0x5040);

        case 0xEC80:
            return (0x5180);

        case 0x12D3:
            return (0x0949);

        case 0x0512:
            return (0x02AA);

        case 0xBA32:
            return (0x40A6);
    }

    return (0x3186);
}

void byteToString(char *buff, uint8_t val)
{
    buff[0] = nibbleToHex(val >> 4);
    buff[1] = nibbleToHex(val);
    buff[2] = '\0';
}

void wordToString(char *buff, uint8_t val)
{
    buff[0] = nibbleToHex(val >> 12);
    buff[1] = nibbleToHex(val >> 8);
    buff[2] = nibbleToHex(val >> 4);
    buff[3] = nibbleToHex(val);
    buff[4] = '\0';
}

char nibbleToHex(
    uint8_t nibble) // convert a 4-bit nibble to a hexadecimal character
{
    nibble &= 0xF;
    return (nibble > 9 ? nibble - 10 + 'A' : nibble + '0');
}

void logData(uint8_t *data, uint16_t length, const char *prefix)
{
    if (!loggerEnabled) {
        return;
    }

    Serial.print(millis());
    Serial.print(' ');
    if (strlen(prefix) > 0) {
        Serial.print(prefix);
        Serial.print(": ");
    }
    for (uint16_t i = 0; i < length; i++) {
        Serial.print(" [");
        Serial.print(i);
        Serial.print("]");
        Serial.print(data[i]);
        Serial.print(" ");
    }
    Serial.println("");
}

void logData(std::vector<uint8_t> data, const char *prefix)
{
    if (!loggerEnabled) {
        return;
    }

    Serial.print(millis());
    Serial.print(' ');
    if (strlen(prefix) > 0) {
        Serial.print(prefix);
        Serial.print(": ");
    }
    for (uint16_t i = 0; i < data.size(); i++) {
        Serial.print(" [");
        Serial.print(i);
        Serial.print("]");
        Serial.print(data[i]);
        Serial.print(" ");
    }
    Serial.println("");
}

void logSysex(std::vector<uint8_t> data, const char *prefix)
{
    if (!loggerEnabled) {
        return;
    }

    Serial.print(millis());
    Serial.print(' ');
    if (strlen(prefix) > 0) {
        Serial.print(prefix);
        Serial.print("[ ");
    }
    for (uint16_t i = 0; i < data.size(); i++) {
        Serial.print(" [");
        Serial.print(i);
        Serial.print("] ");
        Serial.print("\"");
        printHex(&data[i], 1);
        if ((i + 1) != (uint16_t)data.size()) {
            Serial.print("\", ");
        } else {
            Serial.print("\" ");
        }
    }
    Serial.println("]");
}

void logSysex(uint8_t *data,
              uint16_t length,
              const char *prefix,
              uint16_t offset)
{
    if (!loggerEnabled) {
        return;
    }

    Serial.print(millis());
    Serial.print(' ');
    if (strlen(prefix) > 0) {
        Serial.print(prefix);
        Serial.print("[ ");
    }
    for (uint16_t i = 0; i < length; i++) {
        if ((offset > 0) && (i >= offset)) {
            Serial.print(" [");
            Serial.print(i - offset);
            Serial.print("] ");
        }
        Serial.print("\"");
        printHex(&data[i], 1);
        if ((i + 1) != length) {
            Serial.print("\", ");
        } else {
            Serial.print("\" ");
        }
    }
    Serial.println("]");
}

void logChars(uint8_t *data, uint16_t length)
{
    if (!loggerEnabled) {
        return;
    }

    for (uint16_t i = 0; i < length; i++) {
        Serial.print((char)data[i]);
    }
    Serial.println("");
}

uint8_t getShift(uint8_t value)
{
    if (value & 1) {
        return (0);
    }
    if (value & 2) {
        return (1);
    }
    if (value & 4) {
        return (2);
    }
    if (value & 8) {
        return (3);
    }
    if (value & 16) {
        return (4);
    }
    if (value & 32) {
        return (5);
    }
    if (value & 64) {
        return (6);
    }
    if (value & 128) {
        return (7);
    }
    return (0);
}

uint16_t createMask(uint8_t pos, uint8_t size)
{
    uint16_t mask = 1;

    if (size == 0) {
        return (0);
    }

    for (uint8_t i = 0; i < size - 1; i++) {
        mask = (mask << 1) + 1;
    }
    mask = mask << pos;

    return (mask);
}

void convertToHex(uint8_t data, char *outString)
{
    byte first;

    first = (data >> 4) | 48;
    if (first > 57) {
        outString[0] = first + (byte)7;
    } else {
        outString[0] = first;
    }

    first = (data & 0x0F) | 48;
    if (first > 57) {
        outString[1] = first + (byte)7;
    } else {
        outString[1] = first;
    }
    outString[2] = '\0';
}

void printHex(uint8_t *data, uint8_t length)
{
    char tmp[length * 2 + 1];
    byte first;
    int j = 0;

    for (uint8_t i = 0; i < length; i++) {
        first = (data[i] >> 4) | 48;
        if (first > 57) {
            tmp[j] = first + (byte)7;
        } else {
            tmp[j] = first;
        }
        j++;

        first = (data[i] & 0x0F) | 48;
        if (first > 57) {
            tmp[j] = first + (byte)7;
        } else {
            tmp[j] = first;
        }
        j++;
    }
    tmp[length * 2] = 0;
    Serial.print(tmp);
}

uint16_t getNextPowerOf2(uint16_t value)
{
    uint16_t power = 1;

    while (power < value) {
        power *= 2;
    }

    return (power);
}

uint16_t getRange(uint8_t bitWidth)
{
    return (pow(2, bitWidth));
}

static unsigned char lookup[16] = {
    0x0, 0x8, 0x4, 0xc, 0x2, 0xa, 0x6, 0xe,
    0x1, 0x9, 0x5, 0xd, 0x3, 0xb, 0x7, 0xf,
};

uint8_t reverse(uint8_t n)
{
    // Reverse the top and bottom nibble then swap them.
    return ((lookup[n & 0b1111] << 4) | lookup[n >> 4]);
}

Rectangle slotToBounds(uint8_t slot)
{
    if (slot < 1 || slot > 36) {
        slot = 0;
    } else {
        slot--;
    }

    int row = slot / 6;
    int col = slot % 6;

    return (Rectangle(col_pos[col], row_pos[row], 146, 56));
}

uint8_t boundsToSlot(Rectangle bounds)
{
    uint8_t row = 0;
    uint8_t col = 0;

    for (int i = 0; i < 6; i++) {
        if (col_pos[i] == bounds.getX()) {
            col = i;
        }
        if (row_pos[i] == bounds.getX()) {
            row = i;
        }
    }

    logMessage("row=%d, col=%d", row, col);

    return (row * 6 + col + 1);
}

uint8_t getControlSetIdFromScreenY(uint16_t y)
{
    if (y < 222) {
        return (0);
    } else if (y > 398) {
        return (2);
    }
    return (1);
}
