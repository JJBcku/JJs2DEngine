#pragma once

#include <Miscellaneous/Bool64Def.h>
#include <CustomLists/IDObject.h>

#include <VulkanSimplified/VSCommon/VSDataFormatFlags.h>
#include <VulkanSimplified/VSCommon/VSMemoryAllocationFullID.h>
#include <VulkanSimplified/VSCommon/VSMemoryTypeProperties.h>

#include <VulkanSimplified/VSDevice/VSImageDataLists.h>
#include <VulkanSimplified/VSDevice/VSMemoryObjectsList.h>
#include <VulkanSimplified/VSDevice/VSSynchronizationDataLists.h>
#include <VulkanSimplified/VSDevice/VSWindowList.h>

#include <vector>
#include <string>
#include <stdint.h>
#include <compare>

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

		IDObject<VS::AutoCleanupColorRenderTargetImage> _colorImage;
		IDObject<VS::AutoCleanupImageView> _colorImageView;

		PerFrameData();
	};

	struct RenderImagesData
	{
		uint64_t framesInFlight;
		VS::DataFormatSetIndependentID colorFormat;
		VS::DataFormatSetIndependentID depthFormat;

		uint32_t renderImagesWidth;
		uint32_t renderImagesHeight;

		RenderImagesData();

		std::strong_ordering operator<=>(const RenderImagesData& rhs) const noexcept = default;
		bool operator==(const RenderImagesData& rhs) const noexcept = default;
	};

	class WindowDataInternal
	{
	public:
		WindowDataInternal(const WindowInitializationData& initData, RenderImagesData swapchainData,
			VS::WindowList windowList, VS::SynchronizationDataLists synchroList, VS::ImageDataLists imageList, VS::MemoryObjectsList memoryList);
		~WindowDataInternal();

		void ChangeSwapchainData(RenderImagesData newSwapchainData);

		const PerFrameData& GetFrameData(size_t frameIndex) const;

	private:
		VS::WindowList _windowList;
		VS::SynchronizationDataLists _synchroList;
		VS::ImageDataLists _imageList;
		VS::MemoryObjectsList _memoryList;

		std::string _windowTitle;
		uint32_t _windowWidth;
		uint32_t _windowHeight;

		Misc::Bool64 _fullscreen;

		IDObject<VS::WindowPointer> _windowID;

		RenderImagesData _swapchainData;

		std::vector<VS::MemoryTypeProperties> _colorMemoryProperties;
		std::vector<VS::MemoryTypeProperties> _depthMemoryProperties;

		std::vector<PerFrameData> _perFrameData;
		IDObject<VS::AutoCleanupDepthStencilRenderTargetImage> _depthImage;
		IDObject<VS::AutoCleanupImageView> _depthImageView;

		VS::MemoryAllocationFullID _colorImageMemory;
		VS::MemoryAllocationFullID _depthImageMemory;
	};
}
