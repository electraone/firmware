#pragma once

#include "Colours.h"
#include "Text.h"
#include "Geometry.h"
#include "BitmapTools.h"
#include "BitmapReader.h"
#include "VideoRam.h"

// Hardware configuration
#define RA8876_CS 5
#define RA8876_RESET 6

/**
 *
 */
class Graphics :
    public Text,
    public Geometry,
    public BitmapTools,
    public BitmapReader,
    public VideoRam
{
public:

    Graphics(const uint8_t CSp, const uint8_t RSTp = 255);
    ~Graphics() = default;

    void initialise(void);
};
