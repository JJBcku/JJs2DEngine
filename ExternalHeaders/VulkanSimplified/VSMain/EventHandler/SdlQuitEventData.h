#pragma once

#include <stdint.h>

namespace VulkanSimplified
{
	struct SdlQuitEventData
	{
		uint64_t reserved;
		uint64_t timestamp;

		SdlQuitEventData();
		~SdlQuitEventData();
	};
}
