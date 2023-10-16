/*
* Electra One MIDI Controller Firmware
* See COPYRIGHT file at the top of the source tree.
*
* This product includes software developed by the
* Electra One Project (http://electra.one/).
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.
*/

/**
 * @file RuntimeInfo.h
 *
 * @brief A collection of attrbutes to configure run time state of
 *  the System.
 * 
 * @todo This and Context can actually be merged together.
 */

#pragma once

#include <Arduino.h>

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

    uint8_t getUsbDevStatus(void)
    {
        return (eeprom_read_byte((uint8_t *)0x0203));
    }

    void setUsbDevStatus(bool shouldBeEnabled)
    {
        eeprom_write_byte((uint8_t *)0x0203, shouldBeEnabled);
    }

    uint8_t getKeyboardStatus(void)
    {
        return (eeprom_read_byte((uint8_t *)0x0204));
    }

    void setKeyboardStatus(bool shouldBeEnabled)
    {
        eeprom_write_byte((uint8_t *)0x0204, shouldBeEnabled);
    }

    uint8_t getMouseStatus(void)
    {
        return (eeprom_read_byte((uint8_t *)0x0205));
    }

    void setMouseStatus(bool shouldBeEnabled)
    {
        eeprom_write_byte((uint8_t *)0x0205, shouldBeEnabled);
    }

    uint8_t getKeymediaStatus(void)
    {
        return (eeprom_read_byte((uint8_t *)0x0206));
    }

    void setKeymediaStatus(bool shouldBeEnabled)
    {
        eeprom_write_byte((uint8_t *)0x0206, shouldBeEnabled);
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
