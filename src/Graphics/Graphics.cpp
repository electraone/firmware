#include "Graphics.h"

Graphics::Graphics(const uint8_t CSp, const uint8_t RSTp)
    : Text(CSp, RSTp),
      Geometry(CSp, RSTp),
      BitmapTools(CSp, RSTp),
      BitmapReader(CSp, RSTp),
      VideoRam(CSp, RSTp),
      FrameBuffer(CSp, RSTp)
{
}

void Graphics::initialise(void)
{
    init();
    clear();
    setBacklight(true);
    resetExtRam();
}
