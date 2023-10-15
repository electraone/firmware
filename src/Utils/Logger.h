#pragma once

#include <cstdint>

#define LOG_ERROR 0
#define LOG_WARNING 1
#define LOG_INFO 2
#define LOG_TRACE 3
#define LOG_LUA 4

/**
 * Logger class
 *
 * Logger class is used to send debugging messages to the USB host over the
 * USB MIDI protocol.
 */
class Logger
{
public:
    Logger();
    ~Logger() = default;

    void setPort(uint8_t newPort);
    void setLevel(uint8_t newLevel);
    void enable(void);
    void disable(void);
    void setStatus(bool newStatus);
    void write(uint8_t level, const char *format, ...);

private:
    static constexpr uint16_t MaxLogMessageSize = 200;
    uint8_t minimumLevel;
    uint8_t port;
    bool enabled;
};
