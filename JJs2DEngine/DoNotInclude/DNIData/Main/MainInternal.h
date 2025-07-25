#pragma once

#include <stdint.h>
#include <memory>
#include <vector>
#include <optional>
#include <string>

#include <VulkanSimplified/VSMain/VSMain.h>

#include <VulkanSimplified/VSInstance/VSInstance.h>

#include "../../../Include/Common/VersionData.h"

#include "../../../Include/Main/DeviceSettings.h"

namespace JJs2DEngine
{
	struct MainInitializationData;

	struct DeviceData;
	struct DeviceSwapchainSupport;
	struct DeviceTextureSupport;
	struct DeviceDepthStencilSupport;

	class WindowDataInternal;
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

		std::string _dataFolder;

		std::vector<DeviceData> _deviceList;

		std::optional<DeviceSettings> _currentDevicesSettings;

		size_t _graphicsQueueIndex;
		std::optional<size_t> _transferOnlyQueueIndex;

		std::unique_ptr<WindowDataInternal> _windowData;
		std::unique_ptr<RenderDataInternal> _pipelineList;

		void CreateInstance(const MainInitializationData& initData);
		void EnumerateDevices();

		bool CheckSwapchainFormatAvailability(SwapchainFormat format, const DeviceSwapchainSupport& deviceData);
		bool CheckTexturesFormatAvailability(TextureFormat format, const DeviceTextureSupport& deviceData);
		bool CheckDepthFormatAvailability(DepthFormat format, const DeviceDepthStencilSupport& deviceData);
	};
}
