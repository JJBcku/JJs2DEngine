#pragma once

#include "VSCommandBufferUsageDef.h"

namespace VulkanSimplified
{
	enum class CommandBufferUsage : uint64_t
	{
		ONE_USE = 0x1,
		MULTIPLE_USE_NO_SIMULTANEOUS_USAGE = 0x2,
		MULTIPLE_USE_SIMULTANEOUS_USAGE = 0x4,
	};
}