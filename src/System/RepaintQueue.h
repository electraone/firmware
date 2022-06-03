#pragma once

#include <CircularBuffer.h>
#include "Component.h"

extern CircularBuffer<Component *, 500> repaintQueue;
