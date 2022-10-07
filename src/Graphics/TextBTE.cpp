#include "TextBTE.h"
#include "Colours.h"

static const uint16_t lookupSmall[] = {
    0,   4,   10,  19,  27,  38,  49,  53,  58,  63,  71,  79,  84,
    90,  95,  101, 110, 115, 123, 132, 139, 148, 156, 164, 172, 180,
    185, 189, 197, 205, 214, 221, 234, 244, 252, 262, 271, 279, 286,
    296, 305, 309, 315, 323, 331, 342, 351, 362, 370, 381, 389, 398,
    407, 416, 426, 440, 450, 459, 468, 475, 482, 488, 496, 510, 519,
    527, 535, 544, 553, 561, 570, 578, 583, 589, 596, 601, // a - m
    613, 621, 630, 638, 647, 653, 661, 667, 676, 685, 697, 706, 715, // n - z
    723, 729, 733, 740 // {|}~
};

static const uint16_t lookupMedium[] = {
    0,   8,   14,  25,  34,  47,  59,  63,  68,  73,  82,  92,  96,
    101, 105, 112, 121, 127, 137, 146, 156, 165, 174, 183, 192, 202,
    205, 209, 219, 228, 237, 245, 259, 271, 280, 291, 302, 310, 318,
    329, 339, 342, 348, 358, 366, 379, 390, 402, 411, 423, 433, 442,
    452, 463, 474, 490, 501, 511, 521, 526, 532, 539, 549, 562, 571,
    580, 588, 597, 606, 613, 623, 632, 636, 641, 649, 653, // a - m
    666, 675, 685, 694, 703, 710, 718, 725, 734, 743, 757, 766, 776, // n - z
    784, 791, 794, 800 // {|}~
};

static const uint16_t lookupLarge[] = {
    0,   9,   15,  28,  38,  53,  67,  71,  77,  83,  93,  103, 108,
    114, 118, 126, 137, 143, 154, 164, 175, 185, 196, 206, 217, 227,
    231, 236, 246, 256, 267, 275, 292, 305, 316, 327, 339, 348, 358,
    370, 382, 385, 392, 403, 412, 427, 439, 452, 462, 476, 487, 497,
    509, 521, 533, 551, 564, 575, 586, 592, 600, 606, 617, 633, 642,
    653, 661, 672, 682, 691, 701, 711, 715, 721, 731, 734, // a - m
    750, 760, 771, 781, 791, 799, 808, 815, 825, 836, 852, 862, 873, // n - z
    882, 890, 893, 900 // {|}~
};

static const uint8_t charWidthSmall[] = {
    3,  4, 8, 6, 10, 9, 2, 4, 4, 6, 7, 3, 4, 3, 5, 7,
    4,  7, 7, 7, 7,  7, 7, 7, 7, 3, 3, 7, 7, 7, 5, 10,
    8, // A
    6, // B
    7, // C
    7, // D
    5, // E
    5, // F
    8, // G
    7, // H
    2, // I
    4, // J
    7, // K
    5, // L
    9, // M
    7, // N
    9, // O
    6, // P
    9, // Q
    6, // R
    6, // S
    7, // T
    7, // U
    7, // V
    11, // W
    7, // X
    7, // Y
    7, // Z
    3,  5, 3, 7, 7,
    6, // a
    5, // b,
    5, // c,
    7, // d
    6, // e
    4, // f
    7, // g
    6, // h
    2, // i
    4, // j
    5, // k
    3, // l
    10, // m
    6, // n
    7, // o
    5, // p
    7, // q
    3, // r
    6, // s
    4, // t
    7, // u
    6, // v
    9, // w
    6, // x
    6, // y
    6, // z
    5, // {
    2, // |
    5, // }
    7 // ~
};

static const uint8_t charWidthMedium[] = {
    3,  5, 10, 8, 12, 12, 3, 4, 4, 9, 9, 3, 4, 3, 6, 8,
    5,  8, 8,  9, 8,  8,  8, 9, 9, 3, 4, 9, 8, 8, 7, 13,
    11, // A
    9, // B
    10, // C
    10, // D
    7, // E
    7, // F
    10, // G
    10, // H
    3, // I
    5, // J
    9, // K
    7, // L
    12, // M
    10, // N
    11, // O
    8, // P
    11, // Q
    9, // R
    8, // S
    9, // T
    10, // U
    10, // V
    15, // W
    10, // X
    10, // Y
    9, // Z
    4,  7, 5,  8, 9,
    8, // a
    8, // b
    7, // c
    8, // d
    7, // e
    6, // f
    9, // g
    8, // h
    3, // i
    4, // j
    7, // k
    3, // l
    12, // m
    8, // n
    9, // o
    8, // p
    8, // q
    6, // r
    7, // s
    6, // t
    8, // u
    7, // v
    13, // w
    8, // x
    9, // y
    7, // z
    6, // {
    2, // |
    5, // }
    8 // ~
};

static const uint8_t charWidthLarge[] = {
    3,  5,  12, 9,  14, 13, 3, 4, 4, 9, 9, 4, 5, 3,  7, 10,
    5,  10, 9,  10, 9,  10, 9, 9, 9, 3, 4, 9, 9, 10, 7, 16,
    12, // A
    10, // B
    10, // C
    11, // D
    8, // E
    9, // F
    11, // G
    11, // H
    2, // I
    6, // J
    10, // K
    8, // L
    14, // M
    11, // N
    12, // O
    9, // P
    13, // Q
    10, // R
    9, // S
    11, // T
    11, // U
    11, // V
    17, // W
    11, // X
    10, // Y
    10, // Z
    5,  7,  5,  10, 9,
    8, // a
    9, // b
    7, // c
    10, // d
    8, // e
    7, // f
    9, // g
    9, // h
    3, // i
    5, // j
    8, // k
    2, // l
    15, // m
    9, // n
    9, // o
    9, // p
    9, // q
    7, // r
    7, // s
    6, // t
    9, // u
    9, // v
    14, // w
    8, // x
    10, // y
    8, // z
    7, // {
    2, // |
    6, // }
    10 // ~
};

TextBTE::TextBTE(const uint8_t CSp, const uint8_t RSTp) : FrameBuffer(CSp, RSTp)
{
}

void TextBTE::printText(uint16_t x,
                        uint16_t y,
                        const char *text,
                        TextStyle textStyle,
                        uint16_t width,
                        TextAlign align,
                        uint8_t color)
{
    uint16_t xPosition = 0;
    uint32_t address = getCanvasAddress();
    uint8_t size = getCharSize(textStyle);
    uint16_t textWidth = getTextWidth(text, textStyle);

    // drawRect (x, y, width, 20, 0xFFFF);

    if (textWidth > width) {
        textWidth = width;
    }

    if (align == TextAlign::center) {
        xPosition = (width - textWidth) / 2;
    } else if (align == TextAlign::right) {
        xPosition = width - textWidth;
    } else {
        xPosition = 0;
    }

    uint16_t currentWidth = 0;

    for (uint8_t i = 0; text[i] != 0; i++) {
        int8_t index = getCharacterTableIndex(text[i]);
        uint8_t charWidth = getCharacterWidth(text[i], size);
        currentWidth += charWidth;

        // Do not print outside of the bounding box
        if (currentWidth > width) {
            break;
        }

        if (index >= 0) {
            uint16_t charXposition = 0;
            uint16_t charHeight = getCharPxHeight(textStyle);
            uint16_t charYposition = getCharPositionY(textStyle);

            if (charHeight == 13) {
                charXposition = lookupSmall[index];
            } else if (charHeight == 17) {
                charXposition = lookupMedium[index];
            } else {
                charXposition = lookupLarge[index];
            }

            if ((textStyle == TextStyle::largeTransparent)
                || (textStyle == TextStyle::mediumTransparent)
                || (textStyle == TextStyle::smallTransparent)) {
                setBteChromaColor(0x000000);
                bteCopyChroma(FRAME_UI_TOOLKIT,
                              charXposition,
                              charYposition,
                              address,
                              x + xPosition,
                              y,
                              charWidth,
                              charHeight);
            } else if (textStyle == TextStyle::mediumInverseTransparent) {
                setBteChromaColor(0xFFFFFF);
                bteCopyChroma(FRAME_UI_TOOLKIT,
                              charXposition,
                              charYposition,
                              address,
                              x + xPosition,
                              y,
                              charWidth,
                              charHeight);
            } else {
                charYposition += (color * 60);
                setBteChromaColor(chromaColors[color]);
                // Temp fix - do not use chroma. it does not look good for black fonts.
                bteCopy(FRAME_UI_TOOLKIT,
                        charXposition,
                        charYposition,
                        address,
                        x + xPosition,
                        y,
                        charWidth,
                        charHeight);
            }
        }

        xPosition += charWidth;
    }
}

uint16_t TextBTE::getTextWidth(const char *text, TextStyle textStyle)
{
    uint16_t width = 0;
    uint8_t size = getCharSize(textStyle);

    for (uint8_t i = 0; text[i] != 0; i++) {
        width += getCharacterWidth(text[i], size);
    }

    return (width);
}

void TextBTE::textPlaceHolder(uint16_t x,
                              uint16_t y,
                              const char *text,
                              TextStyle style,
                              uint16_t width,
                              TextAlign align)
{
    uint16_t xPosition;
    uint16_t textWidth = getTextWidth(text, style);
    uint8_t charHeight = getCharPxHeight(style);

    if (align == TextAlign::center) {
        xPosition = (width - textWidth) / 2;
    } else if (align == TextAlign::right) {
        xPosition = width - textWidth;
    } else {
        xPosition = 0;
    }

    setForegroundColor(ElectraColours::rgb565NumericBlack);
    fillRect(x + xPosition - 2, y - 1, textWidth + 2, charHeight + 1);
}

// Private

int8_t TextBTE::getCharacterTableIndex(char c)
{
    int8_t index = -1;

    if ((33 <= c) && (c <= 96)) {
        index = c - 33;
    } else if ((96 < c) && (c <= 127)) {
        index = c - 34;
    } else {
        index = -1; // \todo unknown or space. this could be done better
    }

    return (index);
}

uint8_t TextBTE::getCharacterWidth(char c, uint8_t size)
{
    int8_t index = getCharacterTableIndex(c);
    uint8_t width = 0;

    if (index >= 0) {
        if (size == 1) {
            width = charWidthSmall[index];
        } else if (size == 2) {
            width = charWidthMedium[index];
        } else {
            width = charWidthLarge[index];
        }
    } else {
        width = 5; // space or unknown is always 5px
    }

    // Add extra padding
    width += 1;

    return (width);
}

uint16_t TextBTE::getCharPositionY(TextStyle textStyle)
{
    return ((uint16_t)textStyle);
}

uint16_t TextBTE::getCharSize(TextStyle textStyle)
{
    if ((textStyle == TextStyle::smallWhiteOnBlack)
        || (textStyle == TextStyle::smallTransparent)) {
        return (1);
    } else if ((textStyle == TextStyle::largeWhiteOnBlack)
               || (textStyle == TextStyle::largeTransparent)) {
        return (3);
    } else if ((textStyle == TextStyle::mediumWhiteOnBlack)
               || (textStyle == TextStyle::mediumBlackOnWhite)
               || (textStyle == TextStyle::mediumWhiteOnDimmed)
               || (textStyle == TextStyle::mediumTransparent)) {
        return (2);
    }

    return (0);
}

uint16_t TextBTE::getCharPxHeight(TextStyle textStyle)
{
    if ((textStyle == TextStyle::smallWhiteOnBlack)
        || (textStyle == TextStyle::smallTransparent)) {
        return (13);
    } else if ((textStyle == TextStyle::largeWhiteOnBlack)
               || (textStyle == TextStyle::largeTransparent)) {
        return (19);
    } else if ((textStyle == TextStyle::mediumWhiteOnBlack)
               || (textStyle == TextStyle::mediumBlackOnWhite)
               || (textStyle == TextStyle::mediumWhiteOnDimmed)
               || (textStyle == TextStyle::mediumTransparent)) {
        return (17);
    }

    return (0);
}

// The backround colours of the colour version of the texts.
// The colours are used to filter out the text background using
// RA8876 Chroma BTE
uint16_t TextBTE::chromaColors[6] = { 0xFFFF, 0xF2EA, 0xF4A0,
                                      0x54FD, 0x0533, 0xc232 };
