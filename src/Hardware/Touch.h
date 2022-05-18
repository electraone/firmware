#pragma once

#include "FT5x16.h"

#define CTP_INT 20

class Touch : public FT5x16
{
public:
    Touch() : FT5x16(CTP_INT){};
    ~Touch() = default;
};
