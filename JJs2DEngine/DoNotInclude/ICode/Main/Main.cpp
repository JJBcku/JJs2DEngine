#include "MainIpch.h"
#include "../../../Include/Main/Main.h"

#include "../../../Include/Main/InputDataList.h"
#include "../../../Include/Main/VertexDataMain.h"

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

	bool Main::IsWindowClosed() const
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

	void Main::RenderSingleFrame()
	{
		_internal->RenderSingleFrame();
	}

	bool Main::RenderingShouldBePaused() const
	{
		return _internal->RenderingShouldBePaused();
	}

	std::optional<std::pair<size_t, size_t>> Main::TryToAddTextureToPreloadedTexturesTransferList(const std::vector<unsigned char>& data, uint32_t width, uint32_t height)
	{
		return _internal->TryToAddTextureToPreloadedTexturesTransferList(data, width, height);
	}

	std::optional<std::pair<size_t, size_t>> Main::TryToAddTextureToStreamedTexturesTransferList(const std::vector<unsigned char>& data, uint32_t width, uint32_t height)
	{
		return _internal->TryToAddTextureToStreamedTexturesTransferList(data, width, height);
	}

	void Main::TransferPreLoadedTextures()
	{
		_internal->TransferPreLoadedTextures();
	}

	void Main::SetGammaValue(float newGammaValue)
	{
		_internal->SetGammaValue(newGammaValue);
	}

	float Main::GetGammaValue() const
	{
		return _internal->GetGammaValue();
	}

	float Main::TranslatePositionXInPixelsToWindowSizeRatio(int32_t positionX) const
	{
		return _internal->TranslatePositionXInPixelsToWindowSizeRatio(positionX);
	}

	float Main::TranslatePositionYInPixelsToWindowSizeRatio(int32_t positionY) const
	{
		return _internal->TranslatePositionYInPixelsToWindowSizeRatio(positionY);
	}

	InputDataList Main::GetInputDataList()
	{
		return _internal->GetInputDataList();
	}

	const InputDataList Main::GetInputDataList() const
	{
		return _internal->GetInputDataList();
	}

	VertexDataMain Main::GetVertexDataMainList()
	{
		return _internal->GetVertexDataMainList();
	}

	const VertexDataMain Main::GetVertexDataMainList() const
	{
		return _internal->GetVertexDataMainList();
	}

	void Main::ChangeFullscreen(bool newFullscreen)
	{
		_internal->ChangeFullscreen(newFullscreen);
	}

}