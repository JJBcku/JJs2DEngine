#include "MainIpch.h"
#include "../../../Include/Main/Main.h"

#include "../../DNIData/Main/MainInternal.h"

namespace JJs2DEngine
{
	Main::Main(const MainInitializationData& initData) : _internal(std::make_unique<MainInternal>(initData))
	{
	}

	Main::~Main()
	{
	}

	const std::vector<DeviceData>& Main::GetDeviceList() const
	{
		return _internal->GetDeviceList();
	}

	void Main::CreateDevice(const DeviceSettings& deviceSettings)
	{
		_internal->CreateDevice(deviceSettings);
	}

}