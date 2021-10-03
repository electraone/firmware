#include "Arduino.h"

void interrupts()
{
    __enable_irq();
}

void noInterrupts()
{
    __disable_irq()
}
