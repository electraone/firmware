#pragma once

#include "helpers.h"
#include "ListDataItem.h"
#include <cstdint>
#include <string>
#include <map>
#include <vector>

class ListData
{
public:
    ListData() : id(0)
    {
    }

    explicit ListData(uint8_t newId) : id(newId)
    {
    }

	virtual ~ListData() = default;

    uint8_t getId(void) const
    {
        return (id);
    }

	void addItem(uint16_t value, const char *label)
	{
		addItem(value, ListDataItem(value, label));
	}

	void addItem(uint16_t value, ListDataItem item)
	{
		items.push_back(item);
	}

	ListDataItem getByIndex(uint16_t index) const
	{
        return (items[index]);
	}

	uint16_t getNumItems(void) const
	{
		return (items.size());
	}

	uint16_t getMaxIndex(void) const
	{
		return (getNumItems() - 1);
	}

    void print(void) const
    {
        logMessage("id: %d", getId());
        for (const auto &item : items) {
            logMessage("    midiValue: %d, label: %s",
			    item.getValue(), item.getLabel());
        }
    }

private:
    uint8_t id;
    std::vector<ListDataItem> items;
};
