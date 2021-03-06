#include "Colours.h"
#include <string.h>
#include "RA8876.h"

namespace ElectraColours // For backward compatibility
{
    const Colour white{ 0 };
    const Colour red{ 1 };
    const Colour orange{ 2 };
    const Colour blue{ 3 };
    const Colour green{ 4 };
    const Colour purple{ 5 };

    const char *rgb565White{ "FFFFFF" };
    const char *rgb565Red{ "F45C51" };
    const char *rgb565Orange{ "F49500" };
    const char *rgb565Blue{ "529DEC" };
    const char *rgb565Green{ "03A598" };
    const char *rgb565Purple{ "C44795" };

    const uint16_t rgb565NumericWhite{ 0xFFFF };
    const uint16_t rgb565NumericRed{ 0xEAC9 };
    const uint16_t rgb565NumericOrange{ 0xEC80 };
    const uint16_t rgb565NumericBlue{ 0x54FD }; // this is some sort of glitch
    const uint16_t rgb565NumericGreen{ 0x0512 };
    const uint16_t rgb565NumericPurple{ 0xBA32 };

    const uint16_t rgb565NumericDarkerWhite{ 0x3186 };
    const uint16_t rgb565NumericDarkerRed{ 0x5040 };
    const uint16_t rgb565NumericDarkerOrange{ 0x5180 };
    const uint16_t rgb565NumericDarkerBlue{
        0x0949
    }; // this is some sort of glitch
    const uint16_t rgb565NumericDarkerGreen{ 0x02AA };
    const uint16_t rgb565NumericDarkerPurple{ 0x40A6 };

    const uint16_t rgb565NumericLighterWhite{ 0x5ACB };
    const uint16_t rgb565NumericLighterRed{ 0x8082 };
    const uint16_t rgb565NumericLighterOrange{ 0x8260 };
    const uint16_t rgb565NumericLighterBlue{
        0x2A6E
    }; // this is some sort of glitch
    const uint16_t rgb565NumericLighterGreen{ 0x040E };
    const uint16_t rgb565NumericLighterPurple{ 0x816C };

    const uint16_t rgb565NumericDarkWhite{ 0x10A2 };
    const uint16_t rgb565NumericDarkRed{ 0x1820 };
    const uint16_t rgb565NumericDarkOrange{ 0x20A0 };
    const uint16_t rgb565NumericDarkBlue{
        0x0063
    }; // this is some sort of glitch
    const uint16_t rgb565NumericDarkGreen{ 0x0104 };
    const uint16_t rgb565NumericDarkPurple{ 0x1842 };

    // Visual style
    const uint16_t rgb565NumericDimmed{ 0x630C };
    const uint16_t rgb565NumericSelected{ 0xFFFF };
    const uint16_t rgb565NumericPinInactive{ 0x630C };
    const uint16_t rgb565NumericPinActive{ 0xE120 };
    const uint16_t rgb565NumericBlack{ 0x0000 };
    const uint16_t rgb565NumericFrame{ 0x2104 };
    const uint16_t rgb565NumericDarkerFrame{ 0x10A2 };
    const uint16_t rgb565NumericActiveHandle{ 0xFFC0 };
    const uint16_t rgb565NumericCSBackground{ 0x0081 };

    const uint16_t rgb565NumericButtonOutlineInactive{ 0x2104 };
    const uint16_t rgb565NumericButtonOutlineActive{ 0xFFFF };

#define COLOR_BUTTON_OUTLINE_INACTIVE
#define COLOR_BUTTON_OUTLINE_ACTIVE
} // namespace ElectraColours

const Colour ElectraColours::translateColour(const char *rgb565Colour)
{
    if (rgb565Colour) {
        if (strcmp(rgb565Colour, ElectraColours::rgb565Red) == 0) {
            return (ElectraColours::red);
        } else if (strcmp(rgb565Colour, ElectraColours::rgb565Orange) == 0) {
            return (ElectraColours::orange);
        } else if (strcmp(rgb565Colour, ElectraColours::rgb565Blue) == 0) {
            return (ElectraColours::blue);
        } else if (strcmp(rgb565Colour, ElectraColours::rgb565Green) == 0) {
            return (ElectraColours::green);
        } else if (strcmp(rgb565Colour, ElectraColours::rgb565Purple) == 0) {
            return (ElectraColours::purple);
        }
    }
    return (ElectraColours::white);
}

const char *ElectraColours::translateColour(Colour colour)
{
    if (colour == ElectraColours::red) {
        return (ElectraColours::rgb565Red);
    } else if (colour == ElectraColours::orange) {
        return (ElectraColours::rgb565Orange);
    } else if (colour == ElectraColours::blue) {
        return (ElectraColours::rgb565Blue);
    } else if (colour == ElectraColours::green) {
        return (ElectraColours::rgb565Green);
    } else if (colour == ElectraColours::purple) {
        return (ElectraColours::rgb565Purple);
    }
    return (ElectraColours::rgb565White);
}

const uint16_t ElectraColours::getNumericRgb565(Colour colour)
{
    if (colour == ElectraColours::red) {
        return (ElectraColours::rgb565NumericRed);
    } else if (colour == ElectraColours::orange) {
        return (ElectraColours::rgb565NumericOrange);
    } else if (colour == ElectraColours::blue) {
        return (ElectraColours::rgb565NumericBlue);
    } else if (colour == ElectraColours::green) {
        return (ElectraColours::rgb565NumericGreen);
    } else if (colour == ElectraColours::purple) {
        return (ElectraColours::rgb565NumericPurple);
    }
    return (ElectraColours::rgb565NumericWhite);
}

const uint16_t ElectraColours::getNumericRgb565Darker(Colour colour)
{
    if (colour == ElectraColours::red) {
        return (ElectraColours::rgb565NumericDarkerRed);
    } else if (colour == ElectraColours::orange) {
        return (ElectraColours::rgb565NumericDarkerOrange);
    } else if (colour == ElectraColours::blue) {
        return (ElectraColours::rgb565NumericDarkerBlue);
    } else if (colour == ElectraColours::green) {
        return (ElectraColours::rgb565NumericDarkerGreen);
    } else if (colour == ElectraColours::purple) {
        return (ElectraColours::rgb565NumericDarkerPurple);
    }
    return (ElectraColours::rgb565NumericDarkerWhite);
}

const uint16_t ElectraColours::getNumericRgb565Lighter(Colour colour)
{
    if (colour == ElectraColours::red) {
        return (ElectraColours::rgb565NumericLighterRed);
    } else if (colour == ElectraColours::orange) {
        return (ElectraColours::rgb565NumericLighterOrange);
    } else if (colour == ElectraColours::blue) {
        return (ElectraColours::rgb565NumericLighterBlue);
    } else if (colour == ElectraColours::green) {
        return (ElectraColours::rgb565NumericLighterGreen);
    } else if (colour == ElectraColours::purple) {
        return (ElectraColours::rgb565NumericLighterPurple);
    }
    return (ElectraColours::rgb565NumericLighterWhite);
}

const uint16_t ElectraColours::getNumericRgb565Dark(Colour colour)
{
    if (colour == ElectraColours::red) {
        return (ElectraColours::rgb565NumericDarkRed);
    } else if (colour == ElectraColours::orange) {
        return (ElectraColours::rgb565NumericDarkOrange);
    } else if (colour == ElectraColours::blue) {
        return (ElectraColours::rgb565NumericDarkBlue);
    } else if (colour == ElectraColours::green) {
        return (ElectraColours::rgb565NumericDarkGreen);
    } else if (colour == ElectraColours::purple) {
        return (ElectraColours::rgb565NumericDarkPurple);
    }
    return (ElectraColours::rgb565NumericDarkWhite);
}

namespace Colours
{
#if DEFAULT_BPP == 24
    const uint32_t aliceblue{ 0x00f0f8ff };
    const uint32_t antiquewhite{ 0x00faebd7 };
    const uint32_t aqua{ 0x0000ffff };
    const uint32_t aquamarine{ 0x007fffd4 };
    const uint32_t azure{ 0x00f0ffff };
    const uint32_t beige{ 0x00f5f5dc };
    const uint32_t bisque{ 0x00ffe4c4 };
    const uint32_t black{ 0x00000000 };
    const uint32_t blanchedalmond{ 0x00ffebcd };
    const uint32_t blue{ 0x000000ff };
    const uint32_t blueviolet{ 0x008a2be2 };
    const uint32_t brown{ 0x00a52a2a };
    const uint32_t burlywood{ 0x00deb887 };
    const uint32_t cadetblue{ 0x005f9ea0 };
    const uint32_t chartreuse{ 0x007fff00 };
    const uint32_t chocolate{ 0x00d2691e };
    const uint32_t coral{ 0x00ff7f50 };
    const uint32_t cornflowerblue{ 0x006495ed };
    const uint32_t cornsilk{ 0x00fff8dc };
    const uint32_t crimson{ 0x00dc143c };
    const uint32_t cyan{ 0x0000ffff };
    const uint32_t darkblue{ 0x0000008b };
    const uint32_t darkcyan{ 0x00008b8b };
    const uint32_t darkgoldenrod{ 0x00b8860b };
    const uint32_t darkgrey{ 0x00555555 };
    const uint32_t darkgreen{ 0x00006400 };
    const uint32_t darkkhaki{ 0x00bdb76b };
    const uint32_t darkmagenta{ 0x008b008b };
    const uint32_t darkolivegreen{ 0x00556b2f };
    const uint32_t darkorange{ 0x00ff8c00 };
    const uint32_t darkorchid{ 0x009932cc };
    const uint32_t darkred{ 0x008b0000 };
    const uint32_t darksalmon{ 0x00e9967a };
    const uint32_t darkseagreen{ 0x008fbc8f };
    const uint32_t darkslateblue{ 0x00483d8b };
    const uint32_t darkslategrey{ 0x002f4f4f };
    const uint32_t darkturquoise{ 0x0000ced1 };
    const uint32_t darkviolet{ 0x009400d3 };
    const uint32_t deeppink{ 0x00ff1493 };
    const uint32_t deepskyblue{ 0x0000bfff };
    const uint32_t dimgrey{ 0x00696969 };
    const uint32_t dodgerblue{ 0x001e90ff };
    const uint32_t firebrick{ 0x00b22222 };
    const uint32_t floralwhite{ 0x00fffaf0 };
    const uint32_t forestgreen{ 0x00228b22 };
    const uint32_t fuchsia{ 0x00ff00ff };
    const uint32_t gainsboro{ 0x00dcdcdc };
    const uint32_t ghostwhite{ 0x00f8f8ff };
    const uint32_t gold{ 0x00ffd700 };
    const uint32_t goldenrod{ 0x00daa520 };
    const uint32_t grey{ 0x00808080 };
    const uint32_t green{ 0x00008000 };
    const uint32_t greenyellow{ 0x00adff2f };
    const uint32_t honeydew{ 0x00f0fff0 };
    const uint32_t hotpink{ 0x00ff69b4 };
    const uint32_t indianred{ 0x00cd5c5c };
    const uint32_t indigo{ 0x004b0082 };
    const uint32_t ivory{ 0x00fffff0 };
    const uint32_t khaki{ 0x00f0e68c };
    const uint32_t lavender{ 0x00e6e6fa };
    const uint32_t lavenderblush{ 0x00fff0f5 };
    const uint32_t lawngreen{ 0x007cfc00 };
    const uint32_t lemonchiffon{ 0x00fffacd };
    const uint32_t lightblue{ 0x00add8e6 };
    const uint32_t lightcoral{ 0x00f08080 };
    const uint32_t lightcyan{ 0x00e0ffff };
    const uint32_t lightgoldenrodyellow{ 0x00fafad2 };
    const uint32_t lightgreen{ 0x0090ee90 };
    const uint32_t lightgrey{ 0x00d3d3d3 };
    const uint32_t lightpink{ 0x00ffb6c1 };
    const uint32_t lightsalmon{ 0x00ffa07a };
    const uint32_t lightseagreen{ 0x0020b2aa };
    const uint32_t lightskyblue{ 0x0087cefa };
    const uint32_t lightslategrey{ 0x00778899 };
    const uint32_t lightsteelblue{ 0x00b0c4de };
    const uint32_t lightyellow{ 0x00ffffe0 };
    const uint32_t lime{ 0x0000ff00 };
    const uint32_t limegreen{ 0x0032cd32 };
    const uint32_t linen{ 0x00faf0e6 };
    const uint32_t magenta{ 0x00ff00ff };
    const uint32_t maroon{ 0x00800000 };
    const uint32_t mediumaquamarine{ 0x0066cdaa };
    const uint32_t mediumblue{ 0x000000cd };
    const uint32_t mediumorchid{ 0x00ba55d3 };
    const uint32_t mediumpurple{ 0x009370db };
    const uint32_t mediumseagreen{ 0x003cb371 };
    const uint32_t mediumslateblue{ 0x007b68ee };
    const uint32_t mediumspringgreen{ 0x0000fa9a };
    const uint32_t mediumturquoise{ 0x0048d1cc };
    const uint32_t mediumvioletred{ 0x00c71585 };
    const uint32_t midnightblue{ 0x00191970 };
    const uint32_t mintcream{ 0x00f5fffa };
    const uint32_t mistyrose{ 0x00ffe4e1 };
    const uint32_t moccasin{ 0x00ffe4b5 };
    const uint32_t navajowhite{ 0x00ffdead };
    const uint32_t navy{ 0x00000080 };
    const uint32_t oldlace{ 0x00fdf5e6 };
    const uint32_t olive{ 0x00808000 };
    const uint32_t olivedrab{ 0x006b8e23 };
    const uint32_t orange{ 0x00ffa500 };
    const uint32_t orangered{ 0x00ff4500 };
    const uint32_t orchid{ 0x00da70d6 };
    const uint32_t palegoldenrod{ 0x00eee8aa };
    const uint32_t palegreen{ 0x0098fb98 };
    const uint32_t paleturquoise{ 0x00afeeee };
    const uint32_t palevioletred{ 0x00db7093 };
    const uint32_t papayawhip{ 0x00ffefd5 };
    const uint32_t peachpuff{ 0x00ffdab9 };
    const uint32_t peru{ 0x00cd853f };
    const uint32_t pink{ 0x00ffc0cb };
    const uint32_t plum{ 0x00dda0dd };
    const uint32_t powderblue{ 0x00b0e0e6 };
    const uint32_t purple{ 0x00800080 };
    const uint32_t rebeccapurple{ 0x00663399 };
    const uint32_t red{ 0x00ff0000 };
    const uint32_t rosybrown{ 0x00bc8f8f };
    const uint32_t royalblue{ 0x004169e1 };
    const uint32_t saddlebrown{ 0x008b4513 };
    const uint32_t salmon{ 0x00fa8072 };
    const uint32_t sandybrown{ 0x00f4a460 };
    const uint32_t seagreen{ 0x002e8b57 };
    const uint32_t seashell{ 0x00fff5ee };
    const uint32_t sienna{ 0x00a0522d };
    const uint32_t silver{ 0x00c0c0c0 };
    const uint32_t skyblue{ 0x0087ceeb };
    const uint32_t slateblue{ 0x006a5acd };
    const uint32_t slategrey{ 0x00708090 };
    const uint32_t snow{ 0x00fffafa };
    const uint32_t springgreen{ 0x0000ff7f };
    const uint32_t steelblue{ 0x004682b4 };
    const uint32_t tan{ 0x00d2b48c };
    const uint32_t teal{ 0x00008080 };
    const uint32_t thistle{ 0x00d8bfd8 };
    const uint32_t tomato{ 0x00ff6347 };
    const uint32_t turquoise{ 0x0040e0d0 };
    const uint32_t violet{ 0x00ee82ee };
    const uint32_t wheat{ 0x00f5deb3 };
    const uint32_t white{ 0x00ffffff };
    const uint32_t whitesmoke{ 0x00f5f5f5 };
    const uint32_t yellow{ 0x00ffff00 };
    const uint32_t yellowgreen{ 0x009acd32 };
#else
    const uint32_t aliceblue{ 0xF7DF };
    const uint32_t antiquewhite{ 0xFF5A };
    const uint32_t aqua{ 0x07FF };
    const uint32_t aquamarine{ 0x7FFA };
    const uint32_t azure{ 0xF7FF };
    const uint32_t beige{ 0xF7BB };
    const uint32_t bisque{ 0xFF38 };
    const uint32_t black{ 0x0000 };
    const uint32_t blanchedalmond{ 0xFF59 };
    const uint32_t blue{ 0x001F };
    const uint32_t blueviolet{ 0x895C };
    const uint32_t brown{ 0xA145 };
    const uint32_t burlywood{ 0xDDD0 };
    const uint32_t cadetblue{ 0x5CF4 };
    const uint32_t chartreuse{ 0x7FE0 };
    const uint32_t chocolate{ 0xD343 };
    const uint32_t coral{ 0xFBEA };
    const uint32_t cornflowerblue{ 0x64BD };
    const uint32_t cornsilk{ 0xFFDB };
    const uint32_t crimson{ 0xD8A7 };
    const uint32_t cyan{ 0x07FF };
    const uint32_t darkblue{ 0x0011 };
    const uint32_t darkcyan{ 0x0451 };
    const uint32_t darkgoldenrod{ 0xBC21 };
    const uint32_t darkgrey{ 0x52AA };
    const uint32_t darkgreen{ 0x0320 };
    const uint32_t darkkhaki{ 0xBDAD };
    const uint32_t darkmagenta{ 0x8811 };
    const uint32_t darkolivegreen{ 0x5345 };
    const uint32_t darkorange{ 0xFC60 };
    const uint32_t darkorchid{ 0x9999 };
    const uint32_t darkred{ 0x8800 };
    const uint32_t darksalmon{ 0xECAF };
    const uint32_t darkseagreen{ 0x8DF1 };
    const uint32_t darkslateblue{ 0x49F1 };
    const uint32_t darkslategrey{ 0x2A69 };
    const uint32_t darkturquoise{ 0x067A };
    const uint32_t darkviolet{ 0x901A };
    const uint32_t deeppink{ 0xF8B2 };
    const uint32_t deepskyblue{ 0x05FF };
    const uint32_t dimgrey{ 0x6B4D };
    const uint32_t dodgerblue{ 0x1C9F };
    const uint32_t firebrick{ 0xB104 };
    const uint32_t floralwhite{ 0xFFDE };
    const uint32_t forestgreen{ 0x2444 };
    const uint32_t fuchsia{ 0xF81F };
    const uint32_t gainsboro{ 0xDEFB };
    const uint32_t ghostwhite{ 0xFFDF };
    const uint32_t gold{ 0xFEA0 };
    const uint32_t goldenrod{ 0xDD24 };
    const uint32_t grey{ 0x8410 };
    const uint32_t green{ 0x0400 };
    const uint32_t greenyellow{ 0xAFE5 };
    const uint32_t honeydew{ 0xF7FE };
    const uint32_t hotpink{ 0xFB56 };
    const uint32_t indianred{ 0xCAEB };
    const uint32_t indigo{ 0x4810 };
    const uint32_t ivory{ 0xFFFE };
    const uint32_t khaki{ 0xF731 };
    const uint32_t lavender{ 0xE73F };
    const uint32_t lavenderblush{ 0xFF9E };
    const uint32_t lawngreen{ 0x7FE0 };
    const uint32_t lemonchiffon{ 0xFFD9 };
    const uint32_t lightblue{ 0xAEDC };
    const uint32_t lightcoral{ 0xF410 };
    const uint32_t lightcyan{ 0xE7FF };
    const uint32_t lightgoldenrodyellow{ 0xFFDA };
    const uint32_t lightgreen{ 0x9772 };
    const uint32_t lightgrey{ 0xD69A };
    const uint32_t lightpink{ 0xFDB8 };
    const uint32_t lightsalmon{ 0xFD0F };
    const uint32_t lightseagreen{ 0x2595 };
    const uint32_t lightskyblue{ 0x867F };
    const uint32_t lightslategrey{ 0x7453 };
    const uint32_t lightsteelblue{ 0xB63B };
    const uint32_t lightyellow{ 0xFFFC };
    const uint32_t lime{ 0x07E0 };
    const uint32_t limegreen{ 0x3666 };
    const uint32_t linen{ 0xFF9C };
    const uint32_t magenta{ 0xF81F };
    const uint32_t maroon{ 0x8000 };
    const uint32_t mediumaquamarine{ 0x6675 };
    const uint32_t mediumblue{ 0x0019 };
    const uint32_t mediumorchid{ 0xBABA };
    const uint32_t mediumpurple{ 0x939B };
    const uint32_t mediumseagreen{ 0x3D8E };
    const uint32_t mediumslateblue{ 0x7B5D };
    const uint32_t mediumspringgreen{ 0x07D3 };
    const uint32_t mediumturquoise{ 0x4E99 };
    const uint32_t mediumvioletred{ 0xC0B0 };
    const uint32_t midnightblue{ 0x18CE };
    const uint32_t mintcream{ 0xF7FF };
    const uint32_t mistyrose{ 0xFF3C };
    const uint32_t moccasin{ 0xFF36 };
    const uint32_t navajowhite{ 0xFEF5 };
    const uint32_t navy{ 0x0010 };
    const uint32_t oldlace{ 0xFFBC };
    const uint32_t olive{ 0x8400 };
    const uint32_t olivedrab{ 0x6C64 };
    const uint32_t orange{ 0xFD20 };
    const uint32_t orangered{ 0xFA20 };
    const uint32_t orchid{ 0xDB9A };
    const uint32_t palegoldenrod{ 0xEF55 };
    const uint32_t palegreen{ 0x9FD3 };
    const uint32_t paleturquoise{ 0xAF7D };
    const uint32_t palevioletred{ 0xDB92 };
    const uint32_t papayawhip{ 0xFF7A };
    const uint32_t peachpuff{ 0xFED7 };
    const uint32_t peru{ 0xCC27 };
    const uint32_t pink{ 0xFE19 };
    const uint32_t plum{ 0xDD1B };
    const uint32_t powderblue{ 0xB71C };
    const uint32_t purple{ 0x8010 };
    const uint32_t rebeccapurple{ 0x6193 };
    const uint32_t red{ 0xF800 };
    const uint32_t rosybrown{ 0xBC71 };
    const uint32_t royalblue{ 0x435C };
    const uint32_t saddlebrown{ 0x8A22 };
    const uint32_t salmon{ 0xFC0E };
    const uint32_t sandybrown{ 0xF52C };
    const uint32_t seagreen{ 0x2C4A };
    const uint32_t seashell{ 0xFFBD };
    const uint32_t sienna{ 0xA285 };
    const uint32_t silver{ 0xC618 };
    const uint32_t skyblue{ 0x867D };
    const uint32_t slateblue{ 0x6AD9 };
    const uint32_t slategrey{ 0x7412 };
    const uint32_t snow{ 0xFFDF };
    const uint32_t springgreen{ 0x07EF };
    const uint32_t steelblue{ 0x4416 };
    const uint32_t tan{ 0xD5B1 };
    const uint32_t teal{ 0x0410 };
    const uint32_t thistle{ 0xDDFB };
    const uint32_t tomato{ 0xFB08 };
    const uint32_t turquoise{ 0x471A };
    const uint32_t violet{ 0xEC1D };
    const uint32_t wheat{ 0xF6F6 };
    const uint32_t white{ 0xFFFF };
    const uint32_t whitesmoke{ 0xF7BE };
    const uint32_t yellow{ 0xFFE0 };
    const uint32_t yellowgreen{ 0x9E66 };
#endif

    // Get colour with adjusted brightness
    uint32_t darker(uint32_t originalColour, float factor)
    {
        return (darker(SCREEN_BPP, originalColour, factor));
    }

    uint32_t darker(uint8_t bpp, uint32_t originalColour, float factor)
    {
        if (bpp == 8) {
            return (darker332(originalColour, factor));
        } else if (bpp == 16) {
            return (darker565(originalColour, factor));
        } else {
            return (darker888(originalColour, factor));
        }
    }

    uint32_t darker332(uint32_t originalColour, float factor)
    {
        uint16_t red = (originalColour >> 5) & 0x07;
        uint16_t green = (originalColour >> 2) & 0x07;
        uint16_t blue = originalColour & 0x03;
        uint16_t newRed = (uint16_t)(red * factor) & 0x07;
        uint16_t newGreen = (uint16_t)(green * factor) & 0x07;
        uint16_t newBlue = (uint16_t)(blue * factor) & 0x03;
        return ((newRed << 5) | (newGreen << 2) | newBlue);
    }

    uint32_t darker565(uint32_t originalColour, float factor)
    {
        uint16_t red = (originalColour >> 11) & 0x1f;
        uint16_t green = (originalColour >> 5) & 0x3f;
        uint16_t blue = originalColour & 0x1f;
        uint16_t newRed = (uint16_t)(red * factor) & 0x1f;
        uint16_t newGreen = (uint16_t)(green * factor) & 0x3f;
        uint16_t newBlue = (uint16_t)(blue * factor) & 0x1f;
        return ((newRed << 11) | (newGreen << 5) | newBlue);
    }

    uint32_t darker888(uint32_t originalColour, float factor)
    {
        uint16_t red = (originalColour >> 16) & 0xff;
        uint16_t green = (originalColour >> 8) & 0xff;
        uint16_t blue = originalColour & 0xff;
        uint16_t newRed = (uint16_t)(red * factor) & 0xff;
        uint16_t newGreen = (uint16_t)(green * factor) & 0xff;
        uint16_t newBlue = (uint16_t)(blue * factor) & 0xff;
        return ((newRed << 16) | (newGreen << 8) | newBlue);
    }

    uint32_t lighter(uint32_t originalColour, float factor)
    {
        return (lighter(SCREEN_BPP, originalColour, factor));
    }

    uint32_t lighter(uint8_t bpp, uint32_t originalColour, float factor)
    {
        if (bpp == 8) {
            return (lighter332(originalColour, factor));
        } else if (bpp == 16) {
            return (lighter565(originalColour, factor));
        } else {
            return (lighter888(originalColour, factor));
        }
    }

    uint32_t lighter332(uint32_t originalColour, float factor)
    {
        uint16_t red = (originalColour >> 5) & 0x07;
        uint16_t green = (originalColour >> 2) & 0x07;
        uint16_t blue = originalColour & 0x03;
        uint16_t newRed = (uint16_t)(red + (0x07 - red) * factor) & 0x07;
        uint16_t newGreen = (uint16_t)(green + (0x07 - green) * factor) & 0x07;
        uint16_t newBlue = (uint16_t)(blue + (0x03 - blue) * factor) & 0x03;
        return ((newRed << 5) | (newGreen << 2) | newBlue);
    }

    uint32_t lighter565(uint32_t originalColour, float factor)
    {
        uint16_t red = (originalColour >> 11) & 0x1f;
        uint16_t green = (originalColour >> 5) & 0x3f;
        uint16_t blue = originalColour & 0x1f;
        uint16_t newRed = (uint16_t)(red + (0x1f - red) * factor) & 0x1f;
        uint16_t newGreen = (uint16_t)(green + (0x3f - green) * factor) & 0x3f;
        uint16_t newBlue = (uint16_t)(blue + (0x1f - blue) * factor) & 0x1f;
        return ((newRed << 11) | (newGreen << 5) | newBlue);
    }

    uint32_t lighter888(uint32_t originalColour, float factor)
    {
        uint16_t red = (originalColour >> 16) & 0xff;
        uint16_t green = (originalColour >> 8) & 0xff;
        uint16_t blue = originalColour & 0xff;
        uint16_t newRed = (uint16_t)(red + (0xff - red) * factor) & 0xff;
        uint16_t newGreen = (uint16_t)(green + (0xff - green) * factor) & 0xff;
        uint16_t newBlue = (uint16_t)(blue + (0xff - blue) * factor) & 0xff;
        return ((newRed << 16) | (newGreen << 8) | newBlue);
    }
} // namespace Colours
