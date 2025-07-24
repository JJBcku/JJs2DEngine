#pragma once

#include <stdint.h>
#include <memory>
#include <vector>
#include <optional>

#include <VulkanSimplified/VSMain/VSMain.h>

#include <VulkanSimplified/VSInstance/VSInstance.h>

#include "../../../Include/Common/VersionData.h"

#include "../../../Include/Main/DeviceSettings.h"

namespace JJs2DEngine
{
	struct MainInitializationData;
	struct DeviceData;

	class RenderDataInternal;

	class MainInternal
	{
	public:
		MainInternal(const MainInitializationData& initData);
		~MainInternal();

		const std::vector<DeviceData>& GetDeviceList() const;

		void CreateDevice(const DeviceSettings& deviceSettings);
		//void RecreateDevice(const DeviceSettings& deviceSettings);

	private:
		std::unique_ptr<VS::Main> _VSMain;
		VersionData _minVulkanVersion;
		VersionData _maxVulkanVersion;

		std::vector<DeviceData> _deviceList;

		std::optional<DeviceSettings> _currentDevicesSettings;

		size_t _graphicsQueueIndex;
		std::optional<size_t> _transferOnlyQueueIndex;

		std::unique_ptr<RenderDataInternal> _pipelineList;

		void CreateInstance(const MainInitializationData& initData);
		void EnumerateDevices();
	};
}
