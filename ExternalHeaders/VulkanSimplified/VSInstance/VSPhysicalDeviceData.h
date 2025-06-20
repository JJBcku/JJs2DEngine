#pragma once

#include "VSDeviceVulkan10FeatureFlags.h"

#include <vector>
#include <optional>

namespace VulkanSimplified
{
	class PhysicalDeviceDataInternal;
}

namespace VulkanSimplified
{
	struct DeviceVulkan10Properties;
	struct QueueFamilyData;
	struct FormatsSupportedFeatures;
	struct DeviceExtensionPacksList;
	struct MemoryDataList;
	struct SurfaceSupportData;

	class PhysicalDeviceData
	{
	public:
		PhysicalDeviceData(PhysicalDeviceDataInternal& ref);
		~PhysicalDeviceData();

		const DeviceVulkan10Properties& GetVulkan10Properties() const;
		const DeviceVulkan10FeatureFlags& GetVulkan10Features() const;
		const std::vector<QueueFamilyData>& GetVulkanQueueFamiliesData() const;
		const FormatsSupportedFeatures& GetFormatsSupportedFeatures() const;
		const DeviceExtensionPacksList& GetDeviceExtensionPacks() const;
		const MemoryDataList& GetAvailableMemoryDataList() const;
		const std::optional<SurfaceSupportData>& GetSurfaceSupport() const;

	private:
		PhysicalDeviceDataInternal& _internal;
	};
}