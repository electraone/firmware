#ifndef __INTERVALTIMER_H__
#define __INTERVALTIMER_H__

#include "mk66fx1m0.h"

#ifdef __cplusplus
extern "C" {
#endif

class IntervalTimer {
private:
	static const uint32_t MAX_PERIOD = UINT32_MAX / (F_BUS / 1000000.0);
public:
	IntervalTimer() {
		channel = NULL;
		nvic_priority = 128;
	}
	~IntervalTimer() {
		end();
	}
	bool begin(void (*funct)(), unsigned int microseconds) {
		if (microseconds == 0 || microseconds > MAX_PERIOD) return false;
		uint32_t cycles = (F_BUS / 1000000) * microseconds - 1;
		if (cycles < 36) return false;
		return beginCycles(funct, cycles);
	}
	bool begin(void (*funct)(), int microseconds) {
		if (microseconds < 0) return false;
		return begin(funct, (unsigned int)microseconds);
	}
	bool begin(void (*funct)(), unsigned long microseconds) {
		return begin(funct, (unsigned int)microseconds);
	}
	bool begin(void (*funct)(), long microseconds) {
		return begin(funct, (int)microseconds);
	}
	bool begin(void (*funct)(), float microseconds) {
		if (microseconds <= 0 || microseconds > MAX_PERIOD) return false;
		uint32_t cycles = (float)(F_BUS / 1000000) * microseconds - 0.5;
		if (cycles < 36) return false;
		return beginCycles(funct, cycles);
	}
	bool begin(void (*funct)(), double microseconds) {
		return begin(funct, (float)microseconds);
	}
	void update(unsigned int microseconds) {
		if (microseconds == 0 || microseconds > MAX_PERIOD) return;
		uint32_t cycles = (F_BUS / 1000000) * microseconds - 1;
		if (cycles < 36) return;
		if (channel) channel->LDVAL = cycles;
	}
	void update(int microseconds) {
		if (microseconds < 0) return;
		return update((unsigned int)microseconds);
	}
	void update(unsigned long microseconds) {
		return update((unsigned int)microseconds);
	}
	void update(long microseconds) {
		return update((int)microseconds);
	}
	void update(float microseconds) {
		if (microseconds <= 0 || microseconds > MAX_PERIOD) return;
		uint32_t cycles = (float)(F_BUS / 1000000) * microseconds - 0.5;
		if (cycles < 36) return;
		if (channel) channel->LDVAL = cycles;
	}
	void update(double microseconds) {
		return update((float)microseconds);
	}
	void end();
	void priority(uint8_t n) {
		nvic_priority = n;
		#if defined(KINETISK)
		if (channel) {
			int index = channel - KINETISK_PIT_CHANNELS;
			NVIC_SET_PRIORITY(IRQ_PIT_CH0 + index, nvic_priority);
		}
		#elif defined(KINETISL)
		if (channel) {
			int index = channel - KINETISK_PIT_CHANNELS;
			nvic_priorites[index] = nvic_priority;
			if (nvic_priorites[0] <= nvic_priorites[1]) {
				NVIC_SET_PRIORITY(IRQ_PIT, nvic_priorites[0]);
			} else {
				NVIC_SET_PRIORITY(IRQ_PIT, nvic_priorites[1]);
			}
		}
		#endif
	}
	operator IRQ_NUMBER_t() {
		if (channel) {
			#if defined(KINETISK)
			int index = channel - KINETISK_PIT_CHANNELS;
			return (IRQ_NUMBER_t)(IRQ_PIT_CH0 + index);
			#elif defined(KINETISL)
			return IRQ_PIT;
			#endif
		}
		return (IRQ_NUMBER_t)NVIC_NUM_INTERRUPTS;
	}
private:
	KINETISK_PIT_CHANNEL_t *channel;
	uint8_t nvic_priority;
	#if defined(KINETISL)
	static uint8_t nvic_priorites[2];
	#endif
	bool beginCycles(void (*funct)(), uint32_t cycles);

};


#ifdef __cplusplus
}
#endif

#endif
