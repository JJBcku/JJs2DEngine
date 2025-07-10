#pragma once

#include <memory>
#include <vector>

namespace JJs2DEngine
{
	struct MainInitializationData;
	class MainInternal;

	struct DeviceData;
	struct DeviceSettings;

	class Main
	{
	public:
		Main(const MainInitializationData& initData);
		~Main();

		const std::vector<DeviceData>& GetDeviceList() const;

		void CreateDevice(const DeviceSettings& deviceSettings);
		void RecreateDevice(const DeviceSettings& deviceSettings);

	private:
		std::unique_ptr<MainInternal> _internal;
	};
}