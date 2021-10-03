#include "CppUTest/TestHarness.h"
#include "extram.h"

Graphics screen(RA8876_CS, RA8876_RESET);

TEST_GROUP(ExtramTests){ void setup(){}

                         void teardown(){} };

TEST(ExtramTests, WriteByteValue)
{
    uint32_t address = 0x00000000;
    uint8_t value = 0x7F;
    storeRam8(address, value);
    CHECK_EQUAL(value, readRam8(address));
}
