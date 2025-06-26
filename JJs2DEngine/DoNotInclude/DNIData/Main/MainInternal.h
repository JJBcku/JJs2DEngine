#pragma once

#include <stdint.h>
#include <memory>
#include <vector>

#include <VulkanSimplified/VSMain/VSMain.h>

#include <VulkanSimplified/VSInstance/VSInstance.h>

#include "../../../Include/Common/VersionData.h"

namespace JJs2DEngine
{
	struct MainInitializationData;
	struct DeviceData;

	class MainInternal
	{
	public:
		MainInternal(const MainInitializationData& initData);
		~MainInternal();

		const std::vector<DeviceData>& GetDeviceList() const;

	private:
		std::unique_ptr<VS::Main> _VSMain;
		VersionData _minVulkanVersion;
		VersionData _maxVulkanVersion;

		std::vector<DeviceData> _deviceList;

		void CreateInstance(const MainInitializationData& initData);
		void EnumerateDevices();
	};
}
