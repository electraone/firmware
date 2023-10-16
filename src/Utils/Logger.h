/*
* Electra One MIDI Controller Firmware
* See COPYRIGHT file at the top of the source tree.
*
* This product includes software developed by the
* Electra One Project (http://electra.one/).
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.
*/

/**
 * @file Logger.h
 *
 * @brief An implemention of a USB MIDI Sysex based logger.
 */

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
