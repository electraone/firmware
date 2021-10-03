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
