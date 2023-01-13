#pragma once

#include <cstdint>

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
    void write(const char *format, ...);

private:
    static constexpr uint16_t MaxLogMessageSize = 200;
    uint8_t level;
    uint8_t port;
    bool enabled;
};
