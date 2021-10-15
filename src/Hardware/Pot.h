#pragma once

#include <ADC.h>

extern ADC *adc;

/*
 * Pot class
 *
 * a representation of the physical turning knob
 */
class Pot
{
public:
    Pot(uint8_t id);
    void process(void);
    void initialise(void);

    uint8_t id;
    int16_t step;
    bool active;
    bool touched;

private:
    int16_t A;
    int16_t B;
    int16_t pA;
    int16_t pB;
    uint8_t address;

    static constexpr uint8_t sensitivity = 16;
};
