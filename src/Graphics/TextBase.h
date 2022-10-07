#pragma once

#if DEFAULT_BPP == 24
#define FRAME_UI_TOOLKIT 0x004B0000
#else
#define FRAME_UI_TOOLKIT 0x00258000
#endif

enum class TextAlign { left, center, right };

enum class TextStyle {
    smallWhiteOnBlack = 0,
    mediumWhiteOnBlack = 13,
    mediumBlackOnWhite = 32,
    mediumWhiteOnDimmed = 52,
    largeWhiteOnBlack = 371,
    smallTransparent = 0,
    largeTransparent = 371,
    mediumTransparent = 13,
    mediumInverseTransparent = 32
};
