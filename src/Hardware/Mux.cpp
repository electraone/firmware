#include "Mux.h"
#include "core_pins.h"

void configureMux(void)
{
    pinMode(16, OUTPUT);
    pinMode(17, OUTPUT);
    pinMode(18, OUTPUT);
    pinMode(19, OUTPUT);
    pinMode(49, OUTPUT);
    pinMode(41, INPUT); // button_scan
}

void setMuxAddress(uint8_t address)
{
    // GPIOB_PDOR = potId;

    digitalWriteFast(18, address & 0b1000);
    digitalWriteFast(19, address & 0b0100);
    digitalWriteFast(17, address & 0b0010);
    digitalWriteFast(16, address & 0b0001);
}
