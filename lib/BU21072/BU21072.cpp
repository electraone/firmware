#include "Arduino.h"
#include "BU21072.h"
#include "i2c_t3.h"

BU21072::BU21072 () :
  registerLED (0x00)
{
}

void BU21072::begin ()
{
	pinMode (55, OUTPUT);
	digitalWriteFast (55, 1);
	delay (50);

	pinMode (CAP_INT_PIN, INPUT_PULLUP);
	Wire.begin (I2C_MASTER, 0x00, I2C_PINS_47_48, I2C_PULLUP_EXT, 400000);
}

void BU21072::writeRegister (uint8_t regAddress, uint8_t data)
{
	Wire.beginTransmission (BU21078MUV_ADDRESS);
	Wire.write (regAddress);
	Wire.write (data);
	Wire.endTransmission ();
}

uint8_t BU21072::readRegister (uint8_t regAddress)
{
	uint8_t data[8];

	Wire.beginTransmission (BU21078MUV_ADDRESS);
	Wire.write (regAddress);
	Wire.endTransmission ();

	// Read from Slave (string len unknown, request full buffer)
	Wire.requestFrom (BU21078MUV_ADDRESS, 1);

	if (!Wire.getError ()) {
		Wire.read (data, Wire.available ());
	}

	return (data[0]);
}

void BU21072::reset (void)
{
	writeRegister (REG_SRST1, DATA_SRST1);
	writeRegister (REG_SRST2, DATA_SRST2);
}

bool BU21072::init (void)
{
	uint16_t attemptNum = 0;

	while (readRegister (REG_INTERRUPT) != 1)
	{
		delay (10);
		if (attemptNum > 50)
		{
			return (false);
		}
		attemptNum++;
	}

	writeRegister (REG_CLR_INTERRUPT, 0x00);

	/*
	 * Oversampling settings
	 */
	writeRegister (REG_OSTIMES, 4);

	/*
	 * Switch interrupt mask (mask unused pins and matrix)
	 */
	writeRegister (REG_MSK_SW_KEY1, 0x87);
	writeRegister (REG_MSK_SW_MTRX0, 0xFF);
	writeRegister (REG_MSK_SW_MTRX1, 0xFF);
	writeRegister (REG_MSK_SW_MTRX2, 0xFF);
	writeRegister (REG_MSK_SW_MTRX3, 0xFF);
	writeRegister (REG_MSK_SW_MTRX4, 0x0F);

	/*
	 * setting gains
	 */
	writeRegister (REG_GAIN1, 0xFF);
	writeRegister (REG_GAIN2, 0x0F);

	/*
	 * setting thresholds off -> on
	 */
	writeRegister (REG_THRESH_ON0, 200);
	writeRegister (REG_THRESH_ON1, 200);
	writeRegister (REG_THRESH_ON2, 200);

	/*
	 * setting thresholds on -> off
	 */
	writeRegister (REG_THRESH_OFF, 100);

	/*
	 * assigning gain and threshold
	 */
	writeRegister (REG_CFG_SIN0, 0x00);
	writeRegister (REG_CFG_SIN1, 0x00);
	writeRegister (REG_CFG_SIN2, 0x00);
	writeRegister (REG_CFG_SIN3, 0x00);
	writeRegister (REG_CFG_SIN4, 0x00);
	writeRegister (REG_CFG_SIN5, 0x00);
	writeRegister (REG_CFG_SIN6, 0x00);
	writeRegister (REG_CFG_SIN7, 0x00);

	/*
	 * periodical recalibration
	 */
	// writeRegister(REG_LED_CAL, 0x1C);

	/*
	 * Write config, calibrate, and enable
	 */
	writeRegister (REG_CONTROL, CTRL_CFG);
	delay (1);

	writeRegister (REG_CONTROL, CTRL_CAL | CTRL_ACT);
	delay (1);

	/*
	 * make sure all interrupts are cleared
	 */
	writeRegister (REG_CLR_INTERRUPT, 0x00);
	delay (10);

	return (true);
}

void BU21072::clearAllInterrupts (void)
{
	writeRegister (REG_CLR_DETECT_ON0, 0x00);
	writeRegister (REG_CLR_DETECT_ON1, 0x00);
	writeRegister (REG_CLR_DETECT_OFF0, 0x00);
	writeRegister (REG_CLR_DETECT_OFF1, 0x00);
	writeRegister (REG_CLR_DETECT_CONT0, 0x00);
	writeRegister (REG_CLR_DETECT_CONT1, 0x00);
	writeRegister (REG_CLR_INTERRUPT, 0x00);
}

uint16_t BU21072::readSensors (void)
{
	uint16_t sensorsLow;
	uint16_t sensorsHigh;

	sensorsLow = readRegister (REG_STATE_SIN1);
	sensorsHigh = readRegister (REG_STATE_SIN2);

	return ((sensorsHigh * 32) + sensorsLow);
}

uint8_t BU21072::readAdc (uint8_t sensor)
{
	uint8_t intensity;

	intensity = readRegister (sensor);
	return (intensity);
}

void BU21072::LED (uint8_t led, bool on)
{
	if (on == true)
	{
		registerLED |= 1UL << led;
	}
	else
	{
		registerLED &= ~(1UL << led);
	}
	writeRegister (REG_LED_CH, registerLED);
}
