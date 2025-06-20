#pragma once

#include <stdint.h>

namespace VulkanSimplified
{
	enum class PipelinePolygonMode : uint64_t
	{
		FILL = 1,
		LINE = 2,
		POINT = 3,
	};
}