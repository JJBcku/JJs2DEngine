#pragma once

#include <stdint.h>

namespace VulkanSimplified
{
	struct SdlAppTerminatingEventData
	{
		uint64_t reserved;
		uint64_t timestamp;

		SdlAppTerminatingEventData();
		~SdlAppTerminatingEventData();
	};

	struct SdlAppLowMemoryEventData
	{
		uint64_t reserved;
		uint64_t timestamp;

		SdlAppLowMemoryEventData();
		~SdlAppLowMemoryEventData();
	};

	struct SdlAppWillEnterBackgroundEventData
	{
		uint64_t reserved;
		uint64_t timestamp;

		SdlAppWillEnterBackgroundEventData();
		~SdlAppWillEnterBackgroundEventData();
	};

	struct SdlAppDidEnterBackgroundEventData
	{
		uint64_t reserved;
		uint64_t timestamp;

		SdlAppDidEnterBackgroundEventData();
		~SdlAppDidEnterBackgroundEventData();
	};

	struct SdlAppWillEnterForegroundEventData
	{
		uint64_t reserved;
		uint64_t timestamp;

		SdlAppWillEnterForegroundEventData();
		~SdlAppWillEnterForegroundEventData();
	};

	struct SdlAppDidEnterForegroundEventData
	{
		uint64_t reserved;
		uint64_t timestamp;

		SdlAppDidEnterForegroundEventData();
		~SdlAppDidEnterForegroundEventData();
	};
}