#pragma once

#include <memory>
#include <vector>

#include <Miscellaneous/Bool64Def.h>

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

		Misc::Bool64 IsWindowClosed() const;

		void HandleEvents();

	private:
		std::unique_ptr<MainInternal> _internal;
	};
}