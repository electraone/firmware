#pragma once

#include <stdint.h>
#include "helpers.h"

typedef uint8_t Colour;

namespace ElectraColours
{
    extern const Colour white;
    extern const Colour red;
    extern const Colour orange;
    extern const Colour blue;
    extern const Colour green;
    extern const Colour purple;

    extern const char *rgb565White;
    extern const char *rgb565Red;
    extern const char *rgb565Orange;
    extern const char *rgb565Blue;
    extern const char *rgb565Green;
    extern const char *rgb565Purple;

    extern const uint16_t rgb565NumericWhite;
    extern const uint16_t rgb565NumericRed;
    extern const uint16_t rgb565NumericOrange;
    extern const uint16_t rgb565NumericBlue;
    extern const uint16_t rgb565NumericGreen;
    extern const uint16_t rgb565NumericPurple;

    extern const uint16_t rgb565NumericDarkerWhite;
    extern const uint16_t rgb565NumericDarkerRed;
    extern const uint16_t rgb565NumericDarkerOrange;
    extern const uint16_t rgb565NumericDarkerBlue;
    extern const uint16_t rgb565NumericDarkerGreen;
    extern const uint16_t rgb565NumericDarkerPurple;

    extern const uint16_t rgb565NumericDimmed;
    extern const uint16_t rgb565NumericSelected;
    extern const uint16_t rgb565NumericPinInactive;
    extern const uint16_t rgb565NumericPinActive;
    extern const uint16_t rgb565NumericFrame;
    extern const uint16_t rgb565NumericDarkerFrame;
    extern const uint16_t rgb565NumericActiveHandle;
    extern const uint16_t rgb565NumericCSBackground;
    extern const uint16_t rgb565NumericButtonOutlineInactive;
    extern const uint16_t rgb565NumericButtonOutlineActive;

    extern const uint16_t rgb565NumericWhite;
    extern const uint16_t rgb565NumericBlack;

    const Colour translateColour(const char *rgb565Colour);
    const char *translateColour(Colour colour);
    const uint16_t getNumericRgb565(Colour colour);
    const uint16_t getNumericRgb565Darker(Colour colour);
    const uint16_t getNumericRgb565Lighter(Colour colour);
    const uint16_t getNumericRgb565Dark(Colour colour);
} // namespace ElectraColours

namespace Colours
{
    extern const uint32_t aliceblue;
    extern const uint32_t antiquewhite;
    extern const uint32_t aqua;
    extern const uint32_t aquamarine;
    extern const uint32_t azure;
    extern const uint32_t beige;
    extern const uint32_t bisque;
    extern const uint32_t black;
    extern const uint32_t blanchedalmond;
    extern const uint32_t blue;
    extern const uint32_t blueviolet;
    extern const uint32_t brown;
    extern const uint32_t burlywood;
    extern const uint32_t cadetblue;
    extern const uint32_t chartreuse;
    extern const uint32_t chocolate;
    extern const uint32_t coral;
    extern const uint32_t cornflowerblue;
    extern const uint32_t cornsilk;
    extern const uint32_t crimson;
    extern const uint32_t cyan;
    extern const uint32_t darkblue;
    extern const uint32_t darkcyan;
    extern const uint32_t darkgoldenrod;
    extern const uint32_t darkgrey;
    extern const uint32_t darkgreen;
    extern const uint32_t darkkhaki;
    extern const uint32_t darkmagenta;
    extern const uint32_t darkolivegreen;
    extern const uint32_t darkorange;
    extern const uint32_t darkorchid;
    extern const uint32_t darkred;
    extern const uint32_t darksalmon;
    extern const uint32_t darkseagreen;
    extern const uint32_t darkslateblue;
    extern const uint32_t darkslategrey;
    extern const uint32_t darkturquoise;
    extern const uint32_t darkviolet;
    extern const uint32_t deeppink;
    extern const uint32_t deepskyblue;
    extern const uint32_t dimgrey;
    extern const uint32_t dodgerblue;
    extern const uint32_t firebrick;
    extern const uint32_t floralwhite;
    extern const uint32_t forestgreen;
    extern const uint32_t fuchsia;
    extern const uint32_t gainsboro;
    extern const uint32_t ghostwhite;
    extern const uint32_t gold;
    extern const uint32_t goldenrod;
    extern const uint32_t grey;
    extern const uint32_t green;
    extern const uint32_t greenyellow;
    extern const uint32_t honeydew;
    extern const uint32_t hotpink;
    extern const uint32_t indianred;
    extern const uint32_t indigo;
    extern const uint32_t ivory;
    extern const uint32_t khaki;
    extern const uint32_t lavender;
    extern const uint32_t lavenderblush;
    extern const uint32_t lawngreen;
    extern const uint32_t lemonchiffon;
    extern const uint32_t lightblue;
    extern const uint32_t lightcoral;
    extern const uint32_t lightcyan;
    extern const uint32_t lightgoldenrodyellow;
    extern const uint32_t lightgreen;
    extern const uint32_t lightgrey;
    extern const uint32_t lightpink;
    extern const uint32_t lightsalmon;
    extern const uint32_t lightseagreen;
    extern const uint32_t lightskyblue;
    extern const uint32_t lightslategrey;
    extern const uint32_t lightsteelblue;
    extern const uint32_t lightyellow;
    extern const uint32_t lime;
    extern const uint32_t limegreen;
    extern const uint32_t linen;
    extern const uint32_t magenta;
    extern const uint32_t maroon;
    extern const uint32_t mediumaquamarine;
    extern const uint32_t mediumblue;
    extern const uint32_t mediumorchid;
    extern const uint32_t mediumpurple;
    extern const uint32_t mediumseagreen;
    extern const uint32_t mediumslateblue;
    extern const uint32_t mediumspringgreen;
    extern const uint32_t mediumturquoise;
    extern const uint32_t mediumvioletred;
    extern const uint32_t midnightblue;
    extern const uint32_t mintcream;
    extern const uint32_t mistyrose;
    extern const uint32_t moccasin;
    extern const uint32_t navajowhite;
    extern const uint32_t navy;
    extern const uint32_t oldlace;
    extern const uint32_t olive;
    extern const uint32_t olivedrab;
    extern const uint32_t orange;
    extern const uint32_t orangered;
    extern const uint32_t orchid;
    extern const uint32_t palegoldenrod;
    extern const uint32_t palegreen;
    extern const uint32_t paleturquoise;
    extern const uint32_t palevioletred;
    extern const uint32_t papayawhip;
    extern const uint32_t peachpuff;
    extern const uint32_t peru;
    extern const uint32_t pink;
    extern const uint32_t plum;
    extern const uint32_t powderblue;
    extern const uint32_t purple;
    extern const uint32_t rebeccapurple;
    extern const uint32_t red;
    extern const uint32_t rosybrown;
    extern const uint32_t royalblue;
    extern const uint32_t saddlebrown;
    extern const uint32_t salmon;
    extern const uint32_t sandybrown;
    extern const uint32_t seagreen;
    extern const uint32_t seashell;
    extern const uint32_t sienna;
    extern const uint32_t silver;
    extern const uint32_t skyblue;
    extern const uint32_t slateblue;
    extern const uint32_t slategrey;
    extern const uint32_t snow;
    extern const uint32_t springgreen;
    extern const uint32_t steelblue;
    extern const uint32_t tan;
    extern const uint32_t teal;
    extern const uint32_t thistle;
    extern const uint32_t tomato;
    extern const uint32_t turquoise;
    extern const uint32_t violet;
    extern const uint32_t wheat;
    extern const uint32_t white;
    extern const uint32_t whitesmoke;
    extern const uint32_t yellow;
    extern const uint32_t yellowgreen;

    // Get colour with adjusted brightness
    uint32_t darker(uint32_t originalColour, float factor);
    uint32_t darker(uint8_t bpp, uint32_t originalColour, float factor);
    uint32_t darker332(uint32_t originalColour, float factor);
    uint32_t darker565(uint32_t originalColour, float factor);
    uint32_t darker888(uint32_t originalColour, float factor);

    uint32_t lighter(uint32_t originalColour, float factor);
    uint32_t lighter(uint8_t bpp, uint32_t originalColour, float factor);
    uint32_t lighter332(uint32_t originalColour, float factor);
    uint32_t lighter565(uint32_t originalColour, float factor);
    uint32_t lighter888(uint32_t originalColour, float factor);
} // namespace Colours
