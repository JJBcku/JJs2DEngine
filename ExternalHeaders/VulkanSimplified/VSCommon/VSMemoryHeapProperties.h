#pragma once

#include "VSMemoryHeapPropertiesDef.h"

namespace VulkanSimplified
{
	enum MemoryHeapPropertyFlags : MemoryHeapProperties
	{
		DEVICE_LOCAL_HEAP = 0x1,
		MULTI_INSTANCE_HEAP = 0X2,
	};
}