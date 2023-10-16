/*
* Electra One MIDI Controller Firmware
* See COPYRIGHT file at the top of the source tree.
*
* This product includes software developed by the
* Electra One Project (http://electra.one/).
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.
*/

/**
 * @file TextBase.h
 *
 * @brief Common data for all types of text objects.
 */

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
