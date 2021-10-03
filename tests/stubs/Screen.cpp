#include "Graphics.h"

Graphics::Graphics(const uint8_t CSp, const uint8_t RSTp)
    : RA8876(CSp, RSTp), writeLayer(1)
{
}

Graphics::~Graphics()
{
}
