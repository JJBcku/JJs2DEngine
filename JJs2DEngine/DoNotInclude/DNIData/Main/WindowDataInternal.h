#pragma once

#include <Miscellaneous/Bool64Def.h>
#include <CustomLists/IDObject.h>

#include <VulkanSimplified/VSCommon/VSDataFormatFlags.h>
#include <VulkanSimplified/VSCommon/VSMemoryAllocationFullID.h>
#include <VulkanSimplified/VSCommon/VSMemoryTypeProperties.h>

#include <VulkanSimplified/VSDevice/VSImageDataLists.h>
#include <VulkanSimplified/VSDevice/VSMemoryObjectsList.h>
#include <VulkanSimplified/VSDevice/VSWindowList.h>
#include <VulkanSimplified/VSDevice/VSWindow.h>
#include <VulkanSimplified/VSDevice/VSDescriptorDataLists.h>

#include <vector>
#include <string>
#include <stdint.h>
#include <compare>
#include <optional>

namespace JJs2DEngine
{
	struct WindowInitializationData;

	struct PerFrameData
	{
		IDObject<VS::AutoCleanupColorRenderTargetImage> colorImage;
		IDObject<VS::AutoCleanupImageView> colorImageView;

		IDObject<VS::AutoCleanupDescriptorSet> gammaCorrectionDescriptorSet;

		PerFrameData();
	};

	struct RenderImagesData
	{
		uint64_t framesInFlight;
		VS::DataFormatSetIndependentID colorFormat;
		VS::DataFormatSetIndependentID depthFormat;

		uint32_t renderImagesWidth;
		uint32_t renderImagesHeight;

		IDObject<VS::AutoCleanupRenderPass> renderPassID;
		IDObject<VS::AutoCleanupDescriptorSetLayout> descriptorLayoutID;

		RenderImagesData();

		std::strong_ordering operator<=>(const RenderImagesData& rhs) const noexcept = default;
		bool operator==(const RenderImagesData& rhs) const noexcept = default;
	};

	class WindowDataInternal
	{
	public:
		WindowDataInternal(const WindowInitializationData& initData, RenderImagesData swapchainData, VS::WindowList windowList, VS::ImageDataLists imageList,
			VS::MemoryObjectsList memoryList, VS::DescriptorDataLists descriptorList);
		~WindowDataInternal();

		void ChangeSwapchainData(RenderImagesData newSwapchainData);

		const PerFrameData& GetFrameData(size_t frameIndex) const;
		IDObject<VS::AutoCleanupFramebuffer> GetFramebufferID(size_t graphicsFrameIndice);

		uint32_t GetRenderWidth() const;
		uint32_t GetRenderHeight() const;

		IDObject<VS::AutoCleanupNIFDescriptorPool> GetGammaCorrectionDescriptorPool();
		IDObject<VS::AutoCleanupDescriptorSet> GetGammaCorrectionDescriptorSet(size_t graphicsFrameIndice);

		void ChangeFullscreen(Misc::Bool64Values newFullscreen);

		bool RenderingShouldBePaused() const;

	private:
		VS::WindowList _windowList;
		VS::ImageDataLists _imageList;
		VS::MemoryObjectsList _memoryList;
		VS::DescriptorDataLists _descriptorList;

		std::optional<VS::Window> _window;

		std::string _windowTitle;
		uint32_t _windowWidth;
		uint32_t _windowHeight;

		Misc::Bool64Values _fullscreen;

		IDObject<VS::WindowPointer> _windowID;

		RenderImagesData _swapchainData;

		std::vector<VS::MemoryTypeProperties> _colorMemoryProperties;
		std::vector<VS::MemoryTypeProperties> _depthMemoryProperties;

		std::vector<PerFrameData> _perFrameData;
		std::vector<IDObject<VS::AutoCleanupFramebuffer>> _framebuffers;
		IDObject<VS::AutoCleanupDepthStencilRenderTargetImage> _depthImage;
		IDObject<VS::AutoCleanupImageView> _depthImageView;

		VS::MemoryAllocationFullID _colorImageMemory;
		VS::MemoryAllocationFullID _depthImageMemory;

		IDObject<VS::AutoCleanupNIFDescriptorPool> _gammaCorrectionDescriptorPool;

		void RedoSwapchain(VS::DataFormatSetIndependentID colorFormat, uint32_t framesInFlight);
		void RedoPerFrameData(VS::DataFormatSetIndependentID colorFormat, uint32_t framesInFlight, uint32_t width, uint32_t height);
		void RedoDepthImage(VS::DataFormatSetIndependentID depthFormat, uint32_t width, uint32_t height);

		void DeleteFramebuffers();
		void RedoFramebuffers(IDObject<VS::AutoCleanupRenderPass> renderPassID, uint32_t width, uint32_t height);
	};
}
