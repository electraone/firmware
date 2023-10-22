
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
 * @file main.cpp
 *
 * @brief Firmware entry function.
 */

#include "HardwareSerial.h"

void initialise(void);
void processEvents(void);
extern "C" void yield (void) __attribute__ ((weak));

extern "C" volatile uint32_t systick_millis_count;

extern "C" int main (void)
{
	initialise ();
	while (1)
	{
		processEvents ();
		yield ();
	}
}

void systick_isr (void)
{
	systick_millis_count++;
}

void yield (void)
{
	static uint8_t running = 0;

	if (running)
	{
		return;
	}
	running = 1;

	if (Serial1.available ())
	{
		serialEvent1 ();
	}
	if (Serial2.available ())
	{
		serialEvent2 ();
	}

	running = 0;
};