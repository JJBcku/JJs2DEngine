#pragma once

#include "VSDeviceVulkan10Limits.h"

#include <string>
#include <array>
#include <stdint.h>

namespace VulkanSimplified
{
	enum class DeviceType : uint64_t
	{
		OTHER,
		INTERGRATED_GPU,
		DISCRETE_GPU,
		VIRTUAL_GPU,
		CPU,
	};

	struct DeviceVulkan10Properties
	{
		uint32_t apiMaxSupportedVersion;
		uint32_t driverVersion;
		uint32_t vendorID;
		uint32_t deviceID;
		DeviceType deviceType;
		std::string deviceName;
		std::array<uint8_t, 16> pipelineCacheUUID;

		DeviceVulkan10Limits limits;

		DeviceVulkan10Properties();
		~DeviceVulkan10Properties();
	};
}
