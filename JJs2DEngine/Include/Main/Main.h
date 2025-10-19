#pragma once

#include <memory>
#include <vector>
#include <optional>
#include <utility>

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

		std::optional<std::pair<size_t, size_t>> TryToAddTextureToPreloadedTexturesTransferList(const std::vector<unsigned char>& data, uint32_t width, uint32_t height);
		std::optional<std::pair<size_t, size_t>> TryToAddTextureToStreamedTexturesTransferList(const std::vector<unsigned char>& data, uint32_t width, uint32_t height);

		void TransferPreLoadedTextures();

		InputDataList GetInputDataList();
		const InputDataList GetInputDataList() const;

		VertexDataMain GetVertexDataMainList();
		const VertexDataMain GetVertexDataMainList() const;

		void ChangeFullscreen(Misc::Bool64Values newFullscreen);

	private:
		std::unique_ptr<MainInternal> _internal;
	};
}