#pragma once

#include <array>

namespace VulkanSimplified
{
	struct MemoryObjectsListInitialCapacities
	{
		std::array<size_t, 32> initialCapacities;

		MemoryObjectsListInitialCapacities();
		~MemoryObjectsListInitialCapacities();
	};
}