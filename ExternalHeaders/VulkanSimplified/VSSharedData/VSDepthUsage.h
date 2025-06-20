#pragma once

#include <stdint.h>

namespace VulkanSimplified
{
	enum class DepthUsage : uint64_t
	{
		TEST = 1,
		WRITE = 2,
		TEST_AND_WRITE = 3,
	};
}