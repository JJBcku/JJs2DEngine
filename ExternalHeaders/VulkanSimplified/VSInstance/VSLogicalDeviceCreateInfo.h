#pragma once

#include "VSDeviceVulkan10FeatureFlags.h"
#include "VSDeviceExtensionPacksList.h"

#include <stdint.h>
#include <vector>

namespace VulkanSimplified
{
	struct QueueCreationData
	{
		uint32_t queuesFamily;
		uint32_t padding;
		std::vector<uint16_t> queuesPriorities;

		QueueCreationData();
	};

	struct LogicalDeviceCreationData
	{
		size_t physicalGPUIndex;
		std::vector<QueueCreationData> queuesCreationInfo;

		DeviceVulkan10FeatureFlags vulkan10EnabledFeatures;
		DeviceExtensionPacksList requestedExtensionPacks;

		LogicalDeviceCreationData();
	};
}