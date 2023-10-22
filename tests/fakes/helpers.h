#pragma once

#include <cstdarg>
#include <vector>
#include <cstdint>
#include "Arduino.h"

#define _HELPERS_H

void System::logger.write(LOG_ERROR, const char *format, ...);
void copyString(char *dest, const char *src, uint16_t maxLength);
