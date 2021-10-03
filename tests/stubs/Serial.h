#pragma once

#include <cstdint>
#include <iostream>

class SerialMock
{
public:
    constexpr SerialMock();

    void begin(long);
    void end();
    int available();
    int read();
    int peek();
    void flush();
    void clear();
    void print(const char *str);
    size_t write(uint8_t c);
    size_t write(const uint8_t *buffer, size_t size);
    size_t write(unsigned long n);
    size_t write(long n);
    size_t write(unsigned int n);
    size_t write(int n);
    int availableForWrite();
};

extern SerialMock Serial;
extern void serialEvent(void);
