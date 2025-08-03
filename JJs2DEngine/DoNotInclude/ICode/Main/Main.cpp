#include "MainIpch.h"
#include "../../../Include/Main/Main.h"

#include "../../../Include/Main/InputDataList.h"

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

	void Main::UpdateCurrentTime()
	{
		_internal->UpdateCurrentTime();
	}

	Misc::Bool64 Main::IsWindowClosed() const
	{
		return _internal->IsWindowClosed();
	}

	void Main::HandleEvents()
	{
		return _internal->HandleEvents();
	}

	void Main::WaitForIdleDevice() const
	{
		_internal->WaitForIdleDevice();
	}

	InputDataList Main::GetInputDataList()
	{
		return _internal->GetInputDataList();
	}

	const InputDataList Main::GetInputDataList() const
	{
		return _internal->GetInputDataList();
	}

}