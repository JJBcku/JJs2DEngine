#pragma once

#include <CustomLists\IDObjectDef.h>

#include <utility>

namespace VulkanSimplified
{
	class MemoryAllocationData;
}

namespace VulkanSimplified
{
	typedef size_t MemoryTypeIndex;
	typedef std::pair<IDObject<MemoryAllocationData>, MemoryTypeIndex> MemoryAllocationFullID;
}