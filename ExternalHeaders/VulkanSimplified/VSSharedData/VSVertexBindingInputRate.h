#pragma once

#include <stdint.h>

namespace VulkanSimplified
{
	enum class VertexBindingInputRate : uint64_t
	{
		VERTEX = 1,
		INSTANCE = 2,
	};
}