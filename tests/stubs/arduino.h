#ifndef _ARDUINO
#define _ARDUINO

#include <cstdint>

typedef uint8_t boolean;
typedef uint8_t byte;

#define false 0
#define true 1

void interrupts(void);
void noInterrupts(void);

#endif /* _ARDUINO */
