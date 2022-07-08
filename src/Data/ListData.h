#pragma once

#include "helpers.h"
#include "ListDataItem.h"
#include <cstdint>
#include <string>
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

    uint16_t getId(void) const
    {
        return (id);
    }

    void addItem(int16_t value, const char *label, const char *bitmap = nullptr)
    {
        items.push_back(ListDataItem(value, label, bitmap));
    }

    ListDataItem getByIndex(uint16_t index) const
    {
        return (items[index]);
    }

    ListDataItem getByValue(int16_t value) const
    {
        for (const auto &item : items) {
            if (item.getValue() == value) {
                return (item);
            }
        }
        return (ListDataItem());
    }

    int16_t getIndexByValue(int16_t value) const
    {
        for (uint16_t i = 0; i < items.size(); i++) {
            if (items[i].getValue() == value) {
                return (i);
            }
        }
        return (-1);
    }

    int16_t getValueByIndex(uint16_t index) const
    {
        return (items[index].getValue());
    }

    uint16_t getNumItems(void) const
    {
        return (items.size());
    }

    uint16_t getMaxIndex(void) const
    {
        return (getNumItems() - 1);
    }

    bool hasItems(void) const
    {
        return (!items.empty());
    }

    bool isEmpty(void) const
    {
        return (items.empty());
    }

    void print(void) const
    {
        logMessage("id: %d", getId());
        for (const auto &item : items) {
            logMessage(
                "    midiValue: %d, label: %s, bitmapEmpty: %d, address=%d",
                item.getValue(),
                item.getLabel(),
                item.isBitmapEmpty(),
                item.getAddress());
        }
    }

private:
    uint16_t id;
    std::vector<ListDataItem> items;
};
