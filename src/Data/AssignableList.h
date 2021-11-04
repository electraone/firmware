#pragma once

#include "ListData.h"

class AssignableList
{
public:
	AssignableList() : items(empty) {};
	virtual ~AssignableList() = default;

    void assignListData(const ListData &newListData)
	{
	    items = newListData;
	}

protected:
    ListData &items;

private:
    static ListData empty;
};
