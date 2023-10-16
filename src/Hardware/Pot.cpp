#include "Hardware.h"
#include "System.h"
#include "Pot.h"
#include "Mux.h"

/** @todo: extract platform specific code to a ADC driver. */

Pot::Pot(uint8_t newId)
    : id(newId),
      step(0),
      active(false),
      touched(false),
      A(0),
      B(0),
      pA(0),
      pB(0),
      address(newId)
{
}

void Pot::process(void)
{
    setMuxAddress(this->address);
    MUX_ENABLE;
    A = Hardware::adc.adc0->analogRead(A0);
    B = Hardware::adc.adc1->analogRead(A16);
    MUX_DISABLE;

    //System::logger.write(LOG_ERROR, "A=%d B=%d", A, B);

    uint16_t move = max(abs(B - pB), abs(A - pA));

    if (move > sensitivity) {
        move /= sensitivity;

        if (A <= 1) {
            step = ((B - pB) > 0) ? -move : move;
        } else if (A == 1023) {
            step = ((B - pB) > 0) ? move : -move;
        } else if (B < 512) {
            step = ((A - pA) > 0) ? move : -move;
        } else if (B > 512) {
            step = ((A - pA) > 0) ? -move : move;
        } else {
            step = 0;
        }

        pA = A;
        pB = B;
    } else {
        step = 0;
    }
}

void Pot::initialise(void)
{
    setMuxAddress(this->address);
    MUX_ENABLE;
    A = Hardware::adc.adc0->analogRead(A0);
    B = Hardware::adc.adc1->analogRead(A16);
    MUX_DISABLE;
    pA = this->A;
    pB = this->B;
    active = false;
}
