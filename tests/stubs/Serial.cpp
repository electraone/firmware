#include "Serial.h"

SerialMock Serial;

constexpr SerialMock::SerialMock()
{
}

void SerialMock::begin(long)
{
}

void SerialMock::end()
{
}

int SerialMock::available()
{
    return 0;
}

int SerialMock::read()
{
    return -1;
}

int SerialMock::peek()
{
    return -1;
}

void SerialMock::flush()
{
}

void SerialMock::clear()
{
}

void SerialMock::print(const char *str)
{
}

size_t SerialMock::write(uint8_t c)
{
    return 1;
}

size_t SerialMock::write(const uint8_t *buffer, size_t size)
{
    return size;
}

size_t SerialMock::write(unsigned long n)
{
    return 1;
}

size_t SerialMock::write(long n)
{
    return 1;
}

size_t SerialMock::write(unsigned int n)
{
    return 1;
}

size_t SerialMock::write(int n)
{
    return 1;
}

int SerialMock::availableForWrite()
{
    return 0;
}
