#pragma once

#include <arduino.h>

class RuntimeInfo
{
public:
    RuntimeInfo()
    {
    }

    void read(void)
    {
        setIfNotSet();
        readElectraInfoSerial();
        readElectraInfoHwRevision();
        readElectraInfoBrightness();
    }

    const char *getElectraInfoSerial(void)
    {
        return (electraInfoSerial);
    }

    uint8_t getElectraInfoHwRevision(void)
    {
        return (electraInfoHwRevision);
    }

    uint16_t getElectraInfoBrightness(void)
    {
        return (electraInfoBrightness);
    }

    void setElectraInfoBrightness(uint16_t newBrightness)
    {
        eeprom_write_byte((uint8_t *)0x0100, newBrightness >> 8);
        eeprom_write_byte((uint8_t *)0x0101, newBrightness & 0xFF);
        electraInfoBrightness = newBrightness;
    }

    uint8_t getLastActivePreset(void)
    {
        return (eeprom_read_byte((uint8_t *)0x0201));
    }

    void setLastActivePreset(uint8_t presetId)
    {
        eeprom_write_byte((uint8_t *)0x0201, presetId);
    }

    uint8_t getLoggerStatus(void)
    {
        return (eeprom_read_byte((uint8_t *)0x0202));
    }

    void setLoggerStatus(bool shouldBeEnabled)
    {
        eeprom_write_byte((uint8_t *)0x0202, shouldBeEnabled);
    }

private:
    void setIfNotSet(void)
    {
        uint8_t appFlag = eeprom_read_byte((uint8_t *)0x0200);

        if (appFlag != 1) {
            eeprom_write_byte((uint8_t *)0x0200, 1);
            setLastActivePreset(0);
            setLoggerStatus(false);
        }
    }

    void readElectraInfoSerial(void)
    {
        electraInfoSerial[0] = eeprom_read_byte((uint8_t *)0x0000);
        electraInfoSerial[1] = eeprom_read_byte((uint8_t *)0x0001);
        electraInfoSerial[2] = eeprom_read_byte((uint8_t *)0x0002);
        electraInfoSerial[3] = eeprom_read_byte((uint8_t *)0x0003);
        electraInfoSerial[4] = eeprom_read_byte((uint8_t *)0x0004);
        electraInfoSerial[5] = eeprom_read_byte((uint8_t *)0x0005);
        electraInfoSerial[6] = eeprom_read_byte((uint8_t *)0x0006);
        electraInfoSerial[7] = eeprom_read_byte((uint8_t *)0x0007);
        electraInfoSerial[8] = '\0';

        if (electraInfoSerial[0] != 'E') {
            electraInfoSerial[0] = 'E';
            electraInfoSerial[1] = 'O';
            electraInfoSerial[2] = '-';
            electraInfoSerial[3] = '0';
            electraInfoSerial[4] = '0';
            electraInfoSerial[5] = '0';
            electraInfoSerial[6] = '0';
            electraInfoSerial[7] = '0';
        }
    }

    void readElectraInfoHwRevision(void)
    {
        electraInfoHwRevision = eeprom_read_byte((uint8_t *)0x000A);

        if (eeprom_read_byte((uint8_t *)0x0000) != 'E') {
            electraInfoHwRevision = 200;
        }
    }

    void readElectraInfoBrightness(void)
    {
        uint8_t msb = eeprom_read_byte((uint8_t *)0x0100);
        uint8_t lsb = eeprom_read_byte((uint8_t *)0x0101);

        electraInfoBrightness = (msb << 8) + lsb;

        if (eeprom_read_byte((uint8_t *)0x0000) != 'E') {
            electraInfoBrightness = 0;
        }
    }

    char electraInfoSerial[11];
    uint8_t electraInfoHwRevision;
    uint16_t electraInfoBrightness;
};
