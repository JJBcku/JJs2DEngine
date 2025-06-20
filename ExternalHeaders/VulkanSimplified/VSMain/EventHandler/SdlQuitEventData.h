#pragma once

#include <stdint.h>

namespace VulkanSimplified
{
	struct SdlQuitEventData
	{
		uint32_t timestamp;

		SdlQuitEventData();
		~SdlQuitEventData();
	};
}
