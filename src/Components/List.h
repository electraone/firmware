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
 * @file List.h
 *
 * @brief A Text / Bitmap Selection List component.
 */

#pragma once

#include "AssignableList.h"
#include "Component.h"
#include "LookAndFeel.h"

class List : virtual public Component, public AssignableList
{
public:
    List(const ListData *newListData);
    virtual ~List() = default;

    void setColour(uint32_t newColour);
    void setIndex(uint16_t newIndex);
    uint16_t getIndex(void) const;

    virtual void onTouchMove(const TouchEvent &touchEvent) override;
    virtual void onTouchDown(const TouchEvent &touchEvent) override;
    virtual void onTouchUp(const TouchEvent &touchEvent) override;

    virtual void onPotChange(const PotEvent &potEvent) override;
    virtual void onPotTouchDown(const PotEvent &potEvent) override;
    virtual void onPotTouchUp(const PotEvent &potEvent) override;

    virtual void resized(void) override;
    virtual void paint(Graphics &g) override;

protected:
    uint32_t colour;
    uint16_t index;

private:
};
