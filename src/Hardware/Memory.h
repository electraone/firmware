#pragma once

#include "MemoryPool.h"
#include "BitmapPool.h"

struct Memory {
    Memory() : stringPool(MemoryPool(SDRAM_PAGE_1, SDRAM_PAGE_SIZE)), bitmapPool(BitmapPool(SDRAM_PAGE_2, SDRAM_PAGE_SIZE)) {}

	MemoryPool stringPool;
	BitmapPool bitmapPool;
};
