#pragma once

#include <stdint.h>

namespace VulkanSimplified
{
	struct SdlLocaleChangeEventData
	{
		uint64_t reserved;
		uint64_t timestamp;

		SdlLocaleChangeEventData();
		~SdlLocaleChangeEventData();
	};
}
