#pragma once

#include "helpers.h"
#include "i2c_t3.h"


/*
 * I2C address of the chip
 */
#define BU21078MUV_ADDRESS      0x5D

/*
 * registers
 */
#define REG_INTERRUPT           0x10
#define REG_STATE_SIN1          0x11
#define REG_STATE_SIN2          0x12
#define REG_SRST1               0x85
#define REG_SRST2               0x8A
#define REG_CFG_SIN0            0xC0
#define REG_CFG_SIN1            0xC1
#define REG_CFG_SIN2            0xC2
#define REG_CFG_SIN3            0xC3
#define REG_CFG_SIN4            0xC4
#define REG_CFG_SIN5            0xC5
#define REG_CFG_SIN6            0xC6
#define REG_CFG_SIN7            0xC7
#define REG_GAIN1               0xC8
#define REG_GAIN2               0xC9
#define REG_THRESH_ON0          0xCA
#define REG_THRESH_ON1          0xCB
#define REG_THRESH_ON2          0xCC
#define REG_THRESH_OFF          0xCD
#define REG_OSTIMES             0xCE
#define REG_CONTTIMES           0xCF
#define REG_MSK_SW_KEY0         0xD0
#define REG_MSK_SW_KEY1         0xD1
#define REG_MSK_SW_MTRX0        0xD2
#define REG_MSK_SW_MTRX1        0xD3
#define REG_MSK_SW_MTRX2        0xD4
#define REG_MSK_SW_MTRX3        0xD5
#define REG_MSK_SW_MTRX4        0xD6
#define REG_MSK_INTERRUPT       0xDF
#define REG_LED_CAL							0xEF
#define REG_CLR_INTERRUPT       0xF0
#define REG_CLR_DETECT_ON0      0xF1
#define REG_CLR_DETECT_ON1      0xF2
#define REG_CLR_DETECT_ONM      0xF3
#define REG_CLR_DETECT_OFF0     0xF4
#define REG_CLR_DETECT_OFF1     0xF5
#define REG_CLR_DETECT_OFFM     0xF6
#define REG_CLR_DETECT_CONT0    0xF4
#define REG_CLR_DETECT_CONT1    0xF5
#define REG_CLR_DETECT_CONTM    0xF6
#define REG_LED_CH              0xFA
#define REG_CONTROL             0xFF

/*
 * predefined constants
 */
#define DATA_SRST1          0x55
#define DATA_SRST2          0xAA

/*
 * LEDs
 */
#define LED0                0x01
#define LED1                0x02
#define LED2                0x04
#define LED3                0x08
#define LED4                0x10
#define LED5                0x20
#define LED6                0x40
#define LED7                0x80

/*
 * CLR_INTERRUPT bits
 */
#define CLR_INI             0x01
#define CLR_CAL             0x02
#define CLR_ERCAL           0x04
#define CLR_PERCAL          0x10

/*
 * CONTROL register bits
 */
#define CTRL_ACT            0x01
#define CTRL_CAL            0x02
#define CTRL_CFG            0x04

/*
 * Pin configuration
 */
#define CAP_INT_PIN         42


class BU21072
{
	public:
		BU21072 ();

		void begin (void);
		void reset (void);
		bool init (void);
		void clearAllInterrupts (void);
		uint16_t readSensors (void);
    uint8_t readAdc (uint8_t sensor);

		void writeRegister (uint8_t regAddress, uint8_t data);
		uint8_t readRegister (uint8_t regAddress);

		void LED (uint8_t led, bool on);

	private:
		uint8_t registerLED;
};
