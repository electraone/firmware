#include "Logger.h"
#include <cstdarg>
#include <cstring>
#include "Arduino.h"

Logger::Logger() : level(0), port(2), enabled(true)
{
}

void Logger::setPort(uint8_t newPort)
{
    port = newPort;
}

void Logger::setLevel(uint8_t newLevel)
{
    level = newLevel;
}

void Logger::enable(void)
{
    enabled = true;
}

void Logger::disable(void)
{
    enabled = false;
}

void Logger::setStatus(bool newStatus)
{
    enabled = newStatus;
}

void Logger::write(const char *format, ...)
{
    if (!enabled) {
        return;
    }

    char buf[MaxLogMessageSize];
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
