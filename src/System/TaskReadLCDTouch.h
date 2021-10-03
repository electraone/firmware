#pragma once

#include "TouchEvent.h"

void readLCDTouch(void);
void triggerComponentCallbacks(Component *component,
                               const TouchEvent &touchEvent);
