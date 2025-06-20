#pragma once

#include "VSDescriptorTypeFlagsDef.h"

namespace VulkanSimplified
{
	enum class DescriptorTypeFlagBits : DescriptorTypeFlags
	{
		SAMPLER = 1,
		COMBINED_IMAGE_SAMPLER = 2,
		SAMPLED_IMAGE = 3,
		STORAGE_IMAGE = 4,
		UNIFORM_TEXEL_BUFFER = 5,
		STORAGE_TEXEL_BUFFER = 6,
		UNIFORM_BUFFER = 7,
		STORAGE_BUFFER = 8,
		UNIFORM_BUFFER_DYNAMIC = 9,
		STORAGE_BUFFER_DYNAMIC = 10,
		INPUT_ATTACHMENT = 11,
	};
}