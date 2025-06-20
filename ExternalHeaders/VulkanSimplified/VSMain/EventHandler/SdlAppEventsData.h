#pragma once

#include <stdint.h>

namespace VulkanSimplified
{
	struct SdlAppTerminatingEventData
	{
		uint32_t timestamp;

		SdlAppTerminatingEventData();
		~SdlAppTerminatingEventData();
	};

	struct SdlAppLowMemoryEventData
	{
		uint32_t timestamp;

		SdlAppLowMemoryEventData();
		~SdlAppLowMemoryEventData();
	};

	struct SdlAppWillEnterBackgroundEventData
	{
		uint32_t timestamp;

		SdlAppWillEnterBackgroundEventData();
		~SdlAppWillEnterBackgroundEventData();
	};

	struct SdlAppDidEnterBackgroundEventData
	{
		uint32_t timestamp;

		SdlAppDidEnterBackgroundEventData();
		~SdlAppDidEnterBackgroundEventData();
	};

	struct SdlAppWillEnterForegroundEventData
	{
		uint32_t timestamp;

		SdlAppWillEnterForegroundEventData();
		~SdlAppWillEnterForegroundEventData();
	};

	struct SdlAppDidEnterForegroundEventData
	{
		uint32_t timestamp;

		SdlAppDidEnterForegroundEventData();
		~SdlAppDidEnterForegroundEventData();
	};
}