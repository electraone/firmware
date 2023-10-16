#pragma once

#include <cstdarg>
#include <vector>
#include <cstdint>
#include "Arduino.h"

#define _HELPERS_H

void System::logger.write(LOG_ERROR, const char *format, ...);
void logData(uint8_t *data, uint16_t length);
void logData(uint8_t *data, uint16_t length, const char *prefix);
void logData(std::vector<uint8_t> data, const char *prefix = "");
void logSysex(uint8_t *data, uint16_t length, const char *prefix = "");
void printHex(uint8_t *data, uint8_t length);
void copyString(char *dest, const char *src, uint16_t maxLength);
