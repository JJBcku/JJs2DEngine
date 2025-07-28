#pragma once

#include <Miscellaneous/Bool64Def.h>
#include <CustomLists/IDObject.h>

#include <VulkanSimplified/VSCommon/VSDataFormatFlags.h>

#include <VulkanSimplified/VSDevice/VSSynchronizationDataLists.h>
#include <VulkanSimplified/VSDevice/VSWindowList.h>

#include <vector>
#include <string>
#include <stdint.h>

namespace JJs2DEngine
{
	struct WindowInitializationData;

	struct PerFrameData
	{
		IDObject<VS::AutoCleanupFence> _transferFinishedFence;
		IDObject<VS::AutoCleanupFence> _renderingFinishedFence;
		IDObject<VS::AutoCleanupSemaphore> _transferFinishedSemaphore;
		IDObject<VS::AutoCleanupSemaphore> _imageAcquiredSemaphore;
		IDObject<VS::AutoCleanupSemaphore> _renderingFinishedSemaphore;

		PerFrameData();
	};

	class WindowDataInternal
	{
	public:
		WindowDataInternal(const WindowInitializationData& initData, uint32_t framesInFlight, VS::DataFormatSetIndependentID format, VS::WindowList windowList,
			VS::SynchronizationDataLists synchroList);
		~WindowDataInternal();

		void ChangeSwapchainFormat(VS::DataFormatSetIndependentID newFormat);

		const PerFrameData& GetFrameData(size_t frameIndex) const;

	private:
		VS::WindowList _windowList;
		VS::SynchronizationDataLists _synchroList;

		std::string _windowTitle;
		uint32_t _windowWidth;
		uint32_t _windowHeight;

		uint64_t _framesInFlight;
		VS::DataFormatSetIndependentID _format;

		Misc::Bool64 _fullscreen;

		IDObject<VS::WindowPointer> _windowID;

		std::vector<PerFrameData> _perFrameData;
	};
}
