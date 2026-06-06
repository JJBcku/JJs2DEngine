#pragma once

#include <stdint.h>

namespace VulkanSimplified
{
	struct SdlClipboardEventData
	{
		uint64_t timestamp;
		uint32_t reserved;
		bool owner;
		uint8_t padding1;
		uint16_t padding2;
		int32_t mime_types_number;
		uint32_t padding3;
		const char** mime_types;

		SdlClipboardEventData();
		~SdlClipboardEventData();
	};
}
