#pragma once

#include <stdint.h>

namespace VulkanSimplified
{
	struct SdlRenderTargetsResetEventData
	{
		uint32_t timestamp;

		SdlRenderTargetsResetEventData();
		~SdlRenderTargetsResetEventData();
	};

	struct SdlRenderDeviceResetEventData
	{
		uint32_t timestamp;

		SdlRenderDeviceResetEventData();
		~SdlRenderDeviceResetEventData();
	};
}
