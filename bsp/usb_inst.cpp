#include <Arduino.h>

usb_serial_class Serial;
usb_midi_class usbMIDI;

void serialEvent () __attribute__((weak));
void serialEvent ()
{
}
