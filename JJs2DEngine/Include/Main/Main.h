#pragma once

#include <memory>
#include <vector>

#include <Miscellaneous/Bool64Def.h>

namespace JJs2DEngine
{
	struct MainInitializationData;
	class MainInternal;
	class InputDataList;
	class VertexDataMain;
	class TextureDataMain;

	struct DeviceData;
	struct DeviceSettings;

	class Main
	{
	public:
		Main(const MainInitializationData& initData);
		~Main();

		const std::vector<DeviceData>& GetDeviceList() const;

		void CreateDevice(const DeviceSettings& deviceSettings);

		void UpdateCurrentTime();

		Misc::Bool64 IsWindowClosed() const;

		void HandleEvents();
		void WaitForIdleDevice() const;

		void RenderSingleFrame();
		bool RenderingShouldBePaused() const;

		void TransferPreLoadedTextures();

		InputDataList GetInputDataList();
		const InputDataList GetInputDataList() const;

		VertexDataMain GetVertexDataMainList();
		const VertexDataMain GetVertexDataMainList() const;

		TextureDataMain GetTextureDataMainList();
		const TextureDataMain GetTextureDataMainList() const;

		void ChangeFullscreen(Misc::Bool64Values newFullscreen);

	private:
		std::unique_ptr<MainInternal> _internal;
	};
}