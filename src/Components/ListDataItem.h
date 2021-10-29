#pragma once

#include <cstdint>
#include "MemoryPool.h"

extern MemoryPool stringPool;

class ListDataItem
{
public:
	ListDataItem() : value(0), address(0) {}

    ListDataItem(uint16_t newValue, const char *newLabel) : address(0)
	{
		value = newValue;
		address = stringPool.saveItem(0, newLabel);
	}

	virtual ~ListDataItem() = default;

    uint16_t getValue(void) const
	{
		return (value);
	}

	const char* getLabel(void) const
	{
        uint32_t bitmapAddress;

		stringPool.getItem (address, buffer, sizeof(buffer), &bitmapAddress);

		return (buffer);
	}

private:
	uint16_t value;
	uint32_t address;
	static char buffer[40];
};
