#include <Arduino.h>
#include "EventResponder.h"

void yield (void) __attribute__ ((weak));
void yield (void)
{
	static uint8_t running = 0;

	if (running)
	{
		return;          // TODO: does this need to be atomic?
	}
	running = 1;
	if (Serial.available ())
	{
		serialEvent ();
	}
	if (Serial1.available ())
	{
		serialEvent1 ();
	}
	if (Serial2.available ())
	{
		serialEvent2 ();
	}

	running = 0;
	EventResponder::runFromYield ();
};
