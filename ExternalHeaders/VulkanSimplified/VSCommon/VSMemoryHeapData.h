#pragma once

#include "VSMemorySizeDef.h"
#include "VSMemoryHeapProperties.h"
#include "VSMemoryTypeData.h"

#include <array>

namespace VulkanSimplified
{
	struct MemoryHeapData
	{
		MemorySize size;
		MemoryHeapProperties properties;

		size_t memoryTypeAmount;
		std::array<MemoryTypeData, 32> memoryTypes;

		MemoryHeapData();
	};
}