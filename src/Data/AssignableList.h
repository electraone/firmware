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

	const ListData& getItems(void)
	{
		return(items);
	}

private:
	ListData &items;
    static ListData empty;
};
