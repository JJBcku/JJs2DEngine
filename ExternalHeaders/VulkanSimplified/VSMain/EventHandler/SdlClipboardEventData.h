#pragma once

#include <stdint.h>

namespace VulkanSimplified
{
	struct SdlClipboardEventData
	{
		uint32_t timestamp;

		SdlClipboardEventData();
		~SdlClipboardEventData();
	};
}
