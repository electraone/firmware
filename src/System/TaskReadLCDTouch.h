#pragma once

#include "TouchEvent.h"
#include "Window.h"

void readLCDTouch(void);
void triggerComponentCallbacks(Window *window,
                               Component *component,
                               const TouchEvent &touchEvent);
