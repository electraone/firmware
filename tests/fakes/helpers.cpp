/*
 * A set of helper functions
 */
#include <iostream>
#include <cstdarg>
#include <cstring>
#include <iostream>
#include "Arduino.h"
#include "helpers.h"

using namespace std;

#define LOG_MESSAGE_MAX_SIZE 1024 // Max size of the log message

void System::logger.write(const char *format, ...)
{
#ifdef DEBUG
    char buf[LOG_MESSAGE_MAX_SIZE];
    va_list ap;

    va_start(ap, format);
    vsnprintf(buf, sizeof(buf), format, ap);
    cout << buf;
    va_end(ap);
    cout << "\n";
#endif
}

void logData(uint8_t *data, uint16_t length)
{
#ifdef DEBUG
    logData(data, length, "");
#endif
}

void logData(uint8_t *data, uint16_t length, const char *prefix)
{
#ifdef DEBUG
    if (strlen(prefix) > 0) {
        cout << prefix;
        cout << ": ";
    }
    for (uint16_t i = 0; i < length; i++) {
        cout << " [";
        cout << i;
        cout << "]";
        cout << data[i];
        cout << " ";
    }
    cout << "\n";
#endif
}

void logData(std::vector<uint8_t> data, const char *prefix)
{
#ifdef DEBUG
    if (strlen(prefix) > 0) {
        cout << prefix;
        cout << ": ";
    }
    for (uint16_t i = 0; i < data.size(); i++) {
        cout << " [";
        cout << i;
        cout << "]";
        cout << data[i];
        cout << " ";
    }
    cout << "\n";
#endif
}

void logSysex(uint8_t *data, uint16_t length, const char *prefix)
{
#ifdef DEBUG
    if (strlen(prefix) > 0) {
        cout << prefix;
        cout << "[ ";
    }
    for (uint16_t i = 0; i < length; i++) {
        cout << "\"";
        printHex(&data[i], 1);
        if ((i + 1) != length) {
            cout << "\", ";
        } else {
            cout << "\" ";
        }
    }
    cout << "]\n";
#endif
}

void logChars(uint8_t *data, uint8_t length)
{
#ifdef DEBUG
    for (uint8_t i = 0; i < length; i++) {
        cout << (char)data[i];
    }
#endif
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
    cout << tmp;
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
