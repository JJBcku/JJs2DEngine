#pragma once

#include "VSMemoryTypePropertiesDef.h"

#include <stdint.h>

namespace VulkanSimplified
{
	struct MemoryTypeData
	{
		uint32_t memoryIndex;
		MemoryTypeProperties properties;

		MemoryTypeData();
	};
}