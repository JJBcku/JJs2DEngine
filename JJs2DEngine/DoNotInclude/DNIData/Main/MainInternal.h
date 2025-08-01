#pragma once

#include <stdint.h>
#include <memory>
#include <vector>
#include <optional>
#include <string>
#include <chrono>
#include <utility>

#include <Miscellaneous/Bool64Def.h>
#include <CustomLists/IDObject.h>
#include <VulkanSimplified/VSMain/EventHandler/SdlEventHandlerTypedefs.h>

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
	class InputDataListInternal;

	class MainInternal
	{
	public:
		MainInternal(const MainInitializationData& initData);
		~MainInternal();

		const std::vector<DeviceData>& GetDeviceList() const;

		void CreateDevice(const DeviceSettings& deviceSettings);
		//void RecreateDevice(const DeviceSettings& deviceSettings);

		void UpdateCurrentTime();

		Misc::Bool64 IsWindowClosed() const;

		void HandleEvents();
		void WaitForIdleDevice() const;

	private:
		std::unique_ptr<VS::Main> _VSMain;
		VersionData _minVulkanVersion;
		VersionData _maxVulkanVersion;

		std::string _dataFolder;

		std::vector<DeviceData> _deviceList;

		std::optional<DeviceSettings> _currentDevicesSettings;

		size_t _graphicsQueueIndex;
		std::optional<size_t> _transferOnlyQueueIndex;

		std::optional<std::chrono::high_resolution_clock::time_point> _lastCurrentTime;
		std::chrono::high_resolution_clock::time_point _currentTime;

		std::unique_ptr<RenderDataInternal> _renderDataList;
		std::unique_ptr<WindowDataInternal> _windowData;
		std::unique_ptr<InputDataListInternal> _inputDataList;

		IDObject<std::pair<VS::QuitEventFunction, void*>> _quitRegistrationID;
		Misc::Bool64 _windowClosed;

		void CreateInstance(const MainInitializationData& initData);
		void EnumerateDevices();

		bool CheckSwapchainFormatAvailability(SwapchainFormat format, const DeviceSwapchainSupport& deviceData);
		bool CheckTexturesFormatAvailability(TextureFormat format, const DeviceTextureSupport& deviceData);
		bool CheckDepthFormatAvailability(DepthFormat format, const DeviceDepthStencilSupport& deviceData);

		bool HandleQuitEvent();

		static bool HandleQuitEventStatic(const VS::SdlQuitEventData&, void* instancePointer);
	};
}
