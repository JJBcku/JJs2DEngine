#pragma once

#include "VSMemoryAllocationFullID.h"
#include "VSMemorySizeDef.h"

#include <utility>

namespace VulkanSimplified
{
	typedef std::pair<MemoryAllocationFullID, MemorySize> MemorySuballocationFullID;
}