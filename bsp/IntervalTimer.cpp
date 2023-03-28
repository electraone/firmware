#include "IntervalTimer.h"
#include "helpers.h"

static void dummy_funct (void);

#define NUM_CHANNELS 4
static void (*funct_table[4])(void) = { dummy_funct, dummy_funct, dummy_funct, dummy_funct };

bool IntervalTimer::beginCycles (void (*funct)(), uint32_t cycles)
{
	if (channel)
	{
		channel->TCTRL = 0;
		channel->TFLG = 1;
	}
	else
	{
		SIM_SCGC6 |= SIM_SCGC6_PIT;
		__asm__ volatile ("nop"); // solves timing problem on Teensy 3.5
		PIT_MCR = 1;
		channel = KINETISK_PIT_CHANNELS;
		while (1)
		{
			if (channel->TCTRL == 0)
			{
				break;
			}
			if (++channel >= KINETISK_PIT_CHANNELS + NUM_CHANNELS)
			{
				channel = NULL;
				return (false);
			}
		}
	}
	int index = channel - KINETISK_PIT_CHANNELS;
	funct_table[index] = funct;
	channel->LDVAL = cycles;
	channel->TCTRL = 3;

	NVIC_SET_PRIORITY (IRQ_PIT_CH0 + index, nvic_priority);
	NVIC_ENABLE_IRQ (IRQ_PIT_CH0 + index);

	return (true);
}


void IntervalTimer::end ()
{
	if (channel)
	{
		int index = channel - KINETISK_PIT_CHANNELS;

		NVIC_DISABLE_IRQ (IRQ_PIT_CH0 + index);
		funct_table[index] = dummy_funct;
		channel->TCTRL = 0;

		channel = 0;
	}
}

void pit0_isr ()
{
	PIT_TFLG0 = 1;
	funct_table[0]();
}

void pit1_isr ()
{
	PIT_TFLG1 = 1;
	funct_table[1]();
}

void pit2_isr ()
{
	PIT_TFLG2 = 1;
	funct_table[2]();
}

void pit3_isr ()
{
	PIT_TFLG3 = 1;
	funct_table[3]();
}

static void dummy_funct (void)
{
}
