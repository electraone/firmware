#ifndef elapsedMillis_h
#define elapsedMillis_h
#ifdef __cplusplus

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class elapsedMillis
{
private:
	unsigned long ms;
public:
	elapsedMillis(void) { ms = millis(); }
	elapsedMillis(unsigned long val) { ms = millis() - val; }
	elapsedMillis(const elapsedMillis &orig) { ms = orig.ms; }
	operator unsigned long () const { return millis() - ms; }
	elapsedMillis & operator = (const elapsedMillis &rhs) { ms = rhs.ms; return *this; }
	elapsedMillis & operator = (unsigned long val) { ms = millis() - val; return *this; }
	elapsedMillis & operator -= (unsigned long val)      { ms += val ; return *this; }
	elapsedMillis & operator += (unsigned long val)      { ms -= val ; return *this; }
	elapsedMillis operator - (int val) const           { elapsedMillis r(*this); r.ms += val; return r; }
	elapsedMillis operator - (unsigned int val) const  { elapsedMillis r(*this); r.ms += val; return r; }
	elapsedMillis operator - (long val) const          { elapsedMillis r(*this); r.ms += val; return r; }
	elapsedMillis operator - (unsigned long val) const { elapsedMillis r(*this); r.ms += val; return r; }
	elapsedMillis operator + (int val) const           { elapsedMillis r(*this); r.ms -= val; return r; }
	elapsedMillis operator + (unsigned int val) const  { elapsedMillis r(*this); r.ms -= val; return r; }
	elapsedMillis operator + (long val) const          { elapsedMillis r(*this); r.ms -= val; return r; }
	elapsedMillis operator + (unsigned long val) const { elapsedMillis r(*this); r.ms -= val; return r; }
};

class elapsedMicros
{
private:
	unsigned long us;
public:
	elapsedMicros(void) { us = micros(); }
	elapsedMicros(unsigned long val) { us = micros() - val; }
	elapsedMicros(const elapsedMicros &orig) { us = orig.us; }
	operator unsigned long () const { return micros() - us; }
	elapsedMicros & operator = (const elapsedMicros &rhs) { us = rhs.us; return *this; }
	elapsedMicros & operator = (unsigned long val) { us = micros() - val; return *this; }
	elapsedMicros & operator -= (unsigned long val)      { us += val ; return *this; }
	elapsedMicros & operator += (unsigned long val)      { us -= val ; return *this; }
	elapsedMicros operator - (int val) const           { elapsedMicros r(*this); r.us += val; return r; }
	elapsedMicros operator - (unsigned int val) const  { elapsedMicros r(*this); r.us += val; return r; }
	elapsedMicros operator - (long val) const          { elapsedMicros r(*this); r.us += val; return r; }
	elapsedMicros operator - (unsigned long val) const { elapsedMicros r(*this); r.us += val; return r; }
	elapsedMicros operator + (int val) const           { elapsedMicros r(*this); r.us -= val; return r; }
	elapsedMicros operator + (unsigned int val) const  { elapsedMicros r(*this); r.us -= val; return r; }
	elapsedMicros operator + (long val) const          { elapsedMicros r(*this); r.us -= val; return r; }
	elapsedMicros operator + (unsigned long val) const { elapsedMicros r(*this); r.us -= val; return r; }
};

#endif // __cplusplus
#endif // elapsedMillis_h
