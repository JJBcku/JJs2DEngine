#pragma once

#include <stdint.h>

namespace VulkanSimplified
{
	struct SdlRenderTargetsResetEventData
	{
		uint64_t reserved;
		uint64_t timestamp;

		SdlRenderTargetsResetEventData();
		~SdlRenderTargetsResetEventData();
	};

	struct SdlRenderDeviceResetEventData
	{
		uint64_t reserved;
		uint64_t timestamp;

		SdlRenderDeviceResetEventData();
		~SdlRenderDeviceResetEventData();
	};
}
