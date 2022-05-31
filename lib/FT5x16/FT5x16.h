#pragma once

#include "CircularBuffer.h"
#include "TouchPoint.h"

#define FT5x16_I2C_ADDRESS 0x38
#define FT5x16_REGISTERS 31
#define FT5x16_TOUCH1_XH 0x03
#define FT5x16_DEVICE_MODE 0x00
#define FT5x16_TD_STATUS 0x02
#define FT5x16_ID_G_CLTR 0x86

class FT5x16
{
public:
    FT5x16(uint8_t newCtpInt);
    void begin(void);
    bool isTouched(void);
    uint8_t readTouch(void);
    bool eventsAvailable(void);
    TouchPoint readEvents(void);
    void clearEvents(void);
    static const char *translateEventToString(TouchPoint::Event event);


private:
    static void isr(void);
    void clearIsr(void);
    void readRegisters(void);
    uint8_t parseTouchPoints(void);
    void writeRegister(uint8_t reg, uint8_t val);

    // Register reading functions
    uint8_t getNumTouches(void);
    uint8_t getId(uint8_t index);
    uint8_t getState(uint8_t index);
    uint16_t getX(uint8_t index);
    uint16_t getY(uint8_t index);
    TouchPoint::Event translateStateToEvent(uint8_t state);

    // TouchPoint functions
    bool isLongHoldPeriod(uint8_t id);
    bool isWithinClickPeriod(uint8_t id);
    bool isWithinDoubleClickPeriod(uint8_t id);
    void resetTouchPoints(void);
    void queueEvent(TouchPoint touchPoint, TouchPoint::Event newEvent);

    static constexpr uint8_t coordRegStart[NUM_TOUCH_POINTS] = { 0x03,
                                                                 0x09,
                                                                 0x0F,
                                                                 0x15,
                                                                 0x1B };

    static volatile bool touched;
    uint8_t ctpInt;
    uint8_t registers[FT5x16_REGISTERS];
    TouchPoint touchPoint[NUM_TOUCH_POINTS];

    CircularBuffer<TouchPoint, NUM_TOUCH_POINTS * 2> queue;
};
