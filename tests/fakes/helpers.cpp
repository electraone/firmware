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

void System::logger.write(LOG_ERROR, const char *format, ...)
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

void copyString(char *dest, const char *src, uint16_t maxLength)
{
    if (src) {
        strncpy(dest, src, maxLength);
        dest[maxLength] = '\0';
    } else {
        dest[0] = '\0';
    }
}
