#pragma once

#include <stdint.h>

namespace VulkanSimplified
{
	struct SdlLocaleChangeEventData
	{
		uint32_t timestamp;

		SdlLocaleChangeEventData();
		~SdlLocaleChangeEventData();
	};
}
