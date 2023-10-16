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
 * @file AssignableList.h
 *
 * @brief An assignable wrapper for Data Lists.
 */

#pragma once

#include "ListData.h"

class AssignableList
{
public:
    AssignableList() : listData(nullptr){};
    AssignableList(const ListData *newListData) : listData(newListData){};
    virtual ~AssignableList() = default;

    void assignListData(const ListData *newListData)
    {
        listData = newListData;
    }

    const ListData *getList(void)
    {
        return (listData);
    }

    bool isEmpty(void) const
    {
        return (listData == nullptr || (listData && listData->isEmpty()));
    }

    bool hasItems(void) const
    {
        return (listData && listData->hasItems());
    }

    bool isAssigned(void) const
    {
        return (listData != nullptr);
    }

private:
    const ListData *listData;
};
