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
