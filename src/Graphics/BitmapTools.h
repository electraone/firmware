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
 * @file BitmapTools.h
 *
 * @brief A collections of functions to manipulate videoRAM bitmaps.
 */

#pragma once

#include "FrameBuffer.h"

/**
 * @brief provides tools to manipulate bitmap graphics
 */
class BitmapTools : public virtual FrameBuffer
{
public:
    BitmapTools(const uint8_t CSp, const uint8_t RSTp);
    ~BitmapTools();

    /**
     * copies rectangular bitmap area from the source to the destination
     * address.
     *
     * @param srcAddress source address.
     * @param srcX X coordinate within the origin given by the srcAddress.
     * @param srcY Y coordinate within the origin given by the srcAddress.
     * @param destAddress destination address.
     * @param destX X coordinate within the origin given by the destAddress.
     * @param destY Y coordinate within the origin given by the destAddress.
     * @param width width of the area to copy.
     * @param height height of the area to copy.
     */
    void copy(uint32_t srcAddress,
              uint16_t srcX,
              uint16_t srcY,
              uint32_t destAddress,
              uint16_t destX,
              uint16_t destY,
              uint16_t width,
              uint16_t height);

    /**
      * copies rectangular bitmap area from the UI toolkit to current
      * Frame buffer working address (ie. buffer with working copy of the
      * screen that is not shown yet).
      *
      * @param srcX X coordinate within the origin given by the srcAddress.
      * @param srcY Y coordinate within the origin given by the srcAddress.
      * @param destX X coordinate within the origin given by the destAddress.
      * @param destY Y coordinate within the origin given by the destAddress.
      * @param width width of the area to copy.
      * @param height height of the area to copy.
      */
    void copyFromToolkit(uint16_t srcX,
                         uint16_t srcY,
                         uint16_t destX,
                         uint16_t destY,
                         uint16_t width,
                         uint16_t height);

    /**
     * darkens the rectangular bitmap area at working Frame buffer, ie. the
     * page that is not shown.
     *
     * @param x X coordinate of the area.
     * @param y Y coordinate of the area.
     * @param width width of the area.
     * @param height height of the area.
     * @param tintColour colour to be applied. Black colour makes the area
     *        darker.
     */
    void dim(uint16_t x,
             uint16_t y,
             uint16_t width,
             uint16_t height,
             uint32_t tintColour);

    /**
    * darkens the rectangular bitmap area at given Frame buffer address.
    *
    * @param x X coordinate of the area.
    * @param y Y coordinate of the area.
    * @param width width of the area.
    * @param height height of the area.
    * @param tintColour colour to be applied. Black colour makes the area
    *        darker.
    */
    void dim(uint32_t address,
             uint16_t x,
             uint16_t y,
             uint16_t width,
             uint16_t height,
             uint32_t tintColour);

    /**
      * darkens whole area of current active window.
      */
    void dimAll(void);

    /**
      * applies a background color the rectangular bitmap area at working
      * Frame buffer, ie. the page that is not shown. The black colour is
      * replaced with given background colour.
      *
      * @param x X coordinate of the area.
      * @param y Y coordinate of the area.
      * @param width width of the area.
      * @param height height of the area.
      * @param tintColour colour to be applied. Black colour makes the area
      *        darker.
      */
    void backdrop(uint16_t x,
                  uint16_t y,
                  uint16_t width,
                  uint16_t height,
                  uint32_t backgroundColour);
};
