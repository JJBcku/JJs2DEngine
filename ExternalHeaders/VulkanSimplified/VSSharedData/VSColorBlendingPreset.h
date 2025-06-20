#pragma once

#include "VSColorBlendingPresetDef.h"

namespace VulkanSimplified
{
	enum class ColorBlendingPreset : uint64_t
	{
		NO_BLENDING = 1,
		ALPHA_BLENDING = 2,
		ADDITIVE_BLENDING = 3,
	};
}