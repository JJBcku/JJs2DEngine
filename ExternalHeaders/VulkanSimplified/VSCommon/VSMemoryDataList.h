#pragma once

#include "VSMemoryHeapData.h"

#include <array>

namespace VulkanSimplified
{
	struct MemoryDataList
	{
		size_t heapAmount;
		std::array<MemoryHeapData, 16> memoryHeaps;

		MemoryDataList();
	};
}