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

private:
    const ListData *listData;
};
