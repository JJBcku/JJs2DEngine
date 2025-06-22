#pragma once

#include "VSMemoryTypePropertiesDef.h"

namespace VulkanSimplified
{
	enum MemoryTypePropertyFlags : MemoryTypeProperties
	{
		DEVICE_LOCAL = 0x1,
		HOST_VISIBLE = 0x2,
		HOST_COHERENT = 0x4,
		HOST_CACHED = 0x8,
		LAZILY_ALLOCATED = 0x10,
		PROTECTED = 0x20,
	};
}