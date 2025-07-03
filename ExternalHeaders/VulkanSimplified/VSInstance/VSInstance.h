#pragma once

namespace VulkanSimplified
{
	class InstanceInternal;
}

namespace VulkanSimplified
{
	class PhysicalDeviceData;
	class DeviceMain;

	struct LogicalDeviceCreationData;
	struct DeviceInitialCapacitiesList;

	class VersionData;

	class Instance
	{
	public:
		Instance(InstanceInternal& ref);
		~Instance();

		Instance& operator=(const Instance&) noexcept = delete;

		const std::string& GetAppFullName() const;
		const std::string& GetEngineFullName() const;

		VersionData GetAppVersion() const;
		VersionData GetEngineVersion() const;

		size_t GetAvailableDevicesCount() const;

		PhysicalDeviceData GetPhysicalDeviceData(size_t deviceIndex);
		DeviceMain GetChoosenDevicesMainClass();

		const PhysicalDeviceData GetPhysicalDeviceData(size_t deviceIndex) const;
		const DeviceMain GetChoosenDevicesMainClass() const;

		void CreateLogicalDevice(const LogicalDeviceCreationData& creationData, const DeviceInitialCapacitiesList& initialCapacities);
		void RecreateLogicalDevice(const LogicalDeviceCreationData& creationData, const DeviceInitialCapacitiesList& initialCapacities);

	private:
		InstanceInternal& _internal;
	};
}