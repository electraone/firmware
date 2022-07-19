#pragma once

#include <CircularBuffer.h>
#include "Component.h"

extern CircularBuffer<Component *, 128> repaintQueue;
