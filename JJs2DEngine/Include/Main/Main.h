#pragma once

#include <memory>
#include <vector>

namespace JJs2DEngine
{
	struct MainInitializationData;
	class MainInternal;

	struct DeviceData;

	class Main
	{
	public:
		Main(const MainInitializationData& initData);
		~Main();

		const std::vector<DeviceData>& GetDeviceList() const;

	private:
		std::unique_ptr<MainInternal> _internal;
	};
}