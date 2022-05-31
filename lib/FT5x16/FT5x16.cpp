#include <SPI.h>
#include "i2c_t3.h"
#include "FT5x16.h"
#include "helpers.h"
#include "setup.h"

volatile bool FT5x16::touched = false;
constexpr uint8_t FT5x16::coordRegStart[5];

FT5x16::FT5x16(uint8_t newCtpInt) : ctpInt(newCtpInt)
{
    resetTouchPoints();
}

void FT5x16::begin(void)
{
    Wire.begin();
    Wire.setClock(400000UL);
    delay(10);
    writeRegister(FT5x16_DEVICE_MODE, 0);
    pinMode(ctpInt, INPUT);
    attachInterrupt(digitalPinToInterrupt(ctpInt), isr, FALLING);
}

void FT5x16::isr(void)
{
    touched = true;
}

void FT5x16::clearIsr(void)
{
    touched = false;
}

bool FT5x16::isTouched(void)
{
    return (touched);
}

uint8_t FT5x16::readTouch(void)
{
    uint8_t numEvents = 0;

    if (isTouched()) {
        readRegisters();
        numEvents = parseTouchPoints();
        clearIsr();
    }

    return (numEvents);
}

uint8_t FT5x16::parseTouchPoints(void)
{
    for (uint8_t i = 0; i < NUM_TOUCH_POINTS; i++) {
        uint8_t state = getState(i);
        TouchPoint::Event event = translateStateToEvent(state);
        uint8_t id = getId(i);

        if (id < NUM_TOUCH_POINTS) {
            if (event == TouchPoint::Start) {
                touchPoint[id].tsStart = millis();
                touchPoint[id].xStart = getX(i);
                touchPoint[id].yStart = getY(i);
            }

            else if (event == TouchPoint::Move) {
                if (isLongHoldPeriod(id)) {
                    if (!touchPoint[id].moved &&
                        touchPoint[id].longHold == false) {
                        touchPoint[id].longHold = true;
                        queueEvent(touchPoint[id], TouchPoint::LongHold);
                    }
                }

                if (!touchPoint[id].isInCloseRange()) {
                    touchPoint[id].moved = true;
                }
            }

            else if (event == TouchPoint::End) {
                if (isWithinDoubleClickPeriod(id)) {
                    queueEvent(touchPoint[id], TouchPoint::DoubleClick);
                } else {
                    if (isWithinClickPeriod(id)) {
                        if (touchPoint[id].isInCloseRange()) {
                            touchPoint[id].tsPreviousClick = millis();
                            queueEvent(touchPoint[id], TouchPoint::Click);
                        }
                    }
                }
            }

            if (event != TouchPoint::None) {
                touchPoint[id].x = getX(i);
                touchPoint[id].y = getY(i);
                touchPoint[id].id = id;
                touchPoint[id].event = event;
                queueEvent(touchPoint[id], event);
            }
        }
    }

    resetTouchPoints();

    return (queue.size());
}

void FT5x16::resetTouchPoints(void)
{
    for (uint8_t i = 0; i < NUM_TOUCH_POINTS; i++) {
        if (touchPoint[i].event == TouchPoint::End) {
            touchPoint[i].event = TouchPoint::None;
            touchPoint[i].longHold = false;
            touchPoint[i].moved = false;
        }
    }
}

void FT5x16::writeRegister(uint8_t reg, uint8_t val)
{
    Wire.beginTransmission(FT5x16_I2C_ADDRESS);
    Wire.write(reg);
    Wire.write(val);
    Wire.endTransmission(FT5x16_I2C_ADDRESS);
}

void FT5x16::readRegisters(void)
{
    Wire.requestFrom(FT5x16_I2C_ADDRESS, FT5x16_REGISTERS);
    uint8_t register_number = 0;

    while (Wire.available()) {
        registers[register_number] = Wire.read();
        register_number++;
    }
}

uint8_t FT5x16::getNumTouches(void)
{
    return (registers[FT5x16_TD_STATUS] & 0xF);
}

uint8_t FT5x16::getId(uint8_t index)
{
    return (registers[coordRegStart[index] + 2] >> 4);
}

uint8_t FT5x16::getState(uint8_t index)
{
    return (registers[coordRegStart[index]] >> 5);
}

uint16_t FT5x16::getX(uint8_t index)
{
    return (SCREEN_WIDTH
            - (((registers[coordRegStart[index]] & 0x0f) << 8)
               | registers[coordRegStart[index] + 1]));
}

uint16_t FT5x16::getY(uint8_t index)
{
    return (SCREEN_HEIGHT
            - (((registers[coordRegStart[index] + 2] & 0x0f) << 8)
               | registers[coordRegStart[index] + 3]));
}

bool FT5x16::isLongHoldPeriod(uint8_t id)
{
    return ((millis() - touchPoint[id].tsStart) > LONGHOLD_PERIOD);
}

bool FT5x16::isWithinClickPeriod(uint8_t id)
{
    return ((millis() - touchPoint[id].tsStart) < CLICK_PERIOD);
}

bool FT5x16::isWithinDoubleClickPeriod(uint8_t id)
{
    return ((millis() - touchPoint[id].tsPreviousClick) < DOUBLECLICK_PERIOD);
}

TouchPoint::Event FT5x16::translateStateToEvent(uint8_t state)
{
    if (state == 0) {
        return (TouchPoint::Start);
    } else if (state == 4) {
        return (TouchPoint::Move);
    } else if (state == 2) {
        return (TouchPoint::End);
    } else {
        return (TouchPoint::None);
    }
}

const char *FT5x16::translateEventToString(TouchPoint::Event event)
{
    if (event == TouchPoint::Start) {
        return ("Start");
    } else if (event == TouchPoint::Move) {
        return ("Move");
    } else if (event == TouchPoint::End) {
        return ("End");
    } else if (event == TouchPoint::LongHold) {
        return ("LongHold");
    } else if (event == TouchPoint::Click) {
        return ("Click");
    } else if (event == TouchPoint::DoubleClick) {
        return ("DoubleClick");
    } else {
        return ("TouchPoint::None");
    }
}

void FT5x16::queueEvent(TouchPoint touchPoint, TouchPoint::Event newEvent)
{
    touchPoint.event = newEvent;
    queue.push(touchPoint);
}

bool FT5x16::eventsAvailable(void)
{
    return (!queue.isEmpty());
}

TouchPoint FT5x16::readEvents(void)
{
    return (queue.shift());
}

void FT5x16::clearEvents(void)
{
    resetTouchPoints();
    queue.clear();
}
