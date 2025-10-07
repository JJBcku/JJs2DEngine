#include "MainDNIpch.h"
#include "WindowDataInternal.h"

#include "../../../Include/Main/WindowInitializationData.h"

#include "DeviceSettingsInternal.h"

#include <Miscellaneous/Bool64.h>

#include <VulkanSimplified/VSCommon/VSImageSampleFlags.h>
#include <VulkanSimplified/VSCommon/VSDataFormatFlags.h>
#include <VulkanSimplified/VSCommon/VSDescriptorTypeFlags.h>
#include <VulkanSimplified/VSCommon/VSImageLayoutFlags.h>

#include <VulkanSimplified/VSDevice/VSWindow.h>
#include <VulkanSimplified/VSDevice/VSMultitypeImagesID.h>

#include <VulkanSimplified/VSDevice/VSWindowCreationData.h>
#include <VulkanSimplified/VSDevice/VSSwapchainCreationData.h>
#include <VulkanSimplified/VSDevice/VSDescriptorSetInputAttachmentWriteData.h>

namespace JJs2DEngine
{
	PerFrameData::PerFrameData()
	{
	}

	RenderImagesData::RenderImagesData()
	{
		framesInFlight = 0;

		renderImagesWidth = 0;
		renderImagesHeight = 0;
	}

	WindowDataInternal::WindowDataInternal(const WindowInitializationData& initData, RenderImagesData swapchainData, VS::WindowList windowList,
		VS::ImageDataLists imageList, VS::MemoryObjectsList memoryList, VS::DescriptorDataLists descriptorList) : _windowList(windowList), _imageList(imageList),
		_memoryList(memoryList), _descriptorList(descriptorList)
	{
		_windowTitle = initData.windowTitle;

		_windowWidth = initData.windowWidth;
		_windowHeight = GetHeight(initData.windowWidth, initData.aspectRatio);

		_fullscreen = initData.fullscreenWindow;

		VS::WindowCreationData windowCreationData;
		windowCreationData.windowTitle = _windowTitle;
		windowCreationData.width = _windowWidth;
		windowCreationData.height = _windowHeight;
		
		if (_fullscreen == Misc::BOOL64_FALSE)
		{
			windowCreationData.settings = VS::WindowSettings::RESIZABLE;
		}
		else if (_fullscreen == Misc::BOOL64_TRUE)
		{
			windowCreationData.settings = VS::WindowSettings::FULLSCREEN_NONEXCLUSIVE;
		}
		else
			throw std::runtime_error("WindowDataInternal Constructor Error: Program was given an erroneous 64bit boolean value!");

		_windowID = _windowList.CreateWindow(windowCreationData);

		_window.emplace(std::move(_windowList.GetWindow(_windowID)));

		VS::SwapchainCreationData swapchainCreationData;

		swapchainCreationData.compositeAlphaMode = VS::COMPOSITE_ALPHA_OPAQUE;
		swapchainCreationData.surfaceTranformMode = VS::SURFACE_TRASFORM_IDENTITY;
		swapchainCreationData.surfacePresentMode = VS::PRESENT_MODE_FIFO_STRICT;
		swapchainCreationData.format = swapchainData.colorFormat;

		swapchainCreationData.imageAmount = static_cast<uint32_t>(swapchainData.framesInFlight);

		_window->CreateSwapchain(swapchainCreationData, true);

		_colorMemoryProperties.reserve(7);
		_colorMemoryProperties.push_back(VS::DEVICE_LOCAL);
		_colorMemoryProperties.push_back(VS::DEVICE_LOCAL | VS::HOST_VISIBLE | VS::HOST_CACHED);
		_colorMemoryProperties.push_back(VS::DEVICE_LOCAL | VS::HOST_COHERENT | VS::HOST_VISIBLE | VS::HOST_CACHED);
		_colorMemoryProperties.push_back(VS::DEVICE_LOCAL | VS::HOST_COHERENT | VS::HOST_VISIBLE);
		_colorMemoryProperties.push_back(VS::HOST_VISIBLE | VS::HOST_CACHED);
		_colorMemoryProperties.push_back(VS::HOST_COHERENT | VS::HOST_VISIBLE | VS::HOST_CACHED);
		_colorMemoryProperties.push_back(VS::HOST_COHERENT | VS::HOST_VISIBLE);

		_depthMemoryProperties.reserve(8);
		_depthMemoryProperties.push_back(VS::DEVICE_LOCAL | VS::LAZILY_ALLOCATED);
		_depthMemoryProperties.push_back(VS::DEVICE_LOCAL);
		_depthMemoryProperties.push_back(VS::DEVICE_LOCAL | VS::HOST_VISIBLE | VS::HOST_CACHED);
		_depthMemoryProperties.push_back(VS::DEVICE_LOCAL | VS::HOST_COHERENT | VS::HOST_VISIBLE | VS::HOST_CACHED);
		_depthMemoryProperties.push_back(VS::DEVICE_LOCAL | VS::HOST_COHERENT | VS::HOST_VISIBLE);
		_depthMemoryProperties.push_back(VS::HOST_VISIBLE | VS::HOST_CACHED);
		_depthMemoryProperties.push_back(VS::HOST_COHERENT | VS::HOST_VISIBLE | VS::HOST_CACHED);
		_depthMemoryProperties.push_back(VS::HOST_COHERENT | VS::HOST_VISIBLE);

		_perFrameData.resize(static_cast<size_t>(swapchainData.framesInFlight));

		for (size_t i = 0; i < _perFrameData.size(); ++i)
		{
			auto& frameData = _perFrameData[i];

			frameData.colorImage = _imageList.AddColorRenderTargetImage(swapchainData.renderImagesWidth, swapchainData.renderImagesHeight, swapchainData.colorFormat,
				VS::SAMPLE_1, {}, false, 1, _perFrameData.size() + 1);
		}

		_depthImage = _imageList.AddDepthStencilRenderTargetImage(swapchainData.renderImagesWidth, swapchainData.renderImagesHeight, swapchainData.depthFormat,
			VS::SAMPLE_1, {}, false, 1, _perFrameData.size() + 1);

		_colorImageMemory = _memoryList.AllocateMemory(imageList.GetColorRenderTargetImagesSize(_perFrameData.back().colorImage) * _perFrameData.size(), _perFrameData.size(),
			_colorMemoryProperties, imageList.GetColorRenderTargetImagesMemoryTypeMask(_perFrameData.back().colorImage), 0x10);

		_depthImageMemory = _memoryList.AllocateMemory(imageList.GetDepthStencilRenderTargetImagesSize(_depthImage), 1, _depthMemoryProperties,
			imageList.GetDepthStencilRenderTargetImagesMemoryTypeMask(_depthImage), 0x10);

		for (size_t i = 0; i < _perFrameData.size(); ++i)
		{
			auto& frameData = _perFrameData[i];
			imageList.BindColorRenderTargetImage(frameData.colorImage, _colorImageMemory);
			frameData.colorImageView = imageList.AddColorRenderTargetImageView(frameData.colorImage);
		}

		imageList.BindDepthStencilRenderTargetImage(_depthImage, _depthImageMemory);
		_depthImageView = imageList.AddDepthStencilRenderTargetImageView(_depthImage);

		_framebuffers.reserve(static_cast<size_t>(swapchainData.framesInFlight));
		std::vector<std::pair<VS::RenderTargetImagesID, IDObject<VS::AutoCleanupImageView>>> attachments;
		attachments.resize(2);
		attachments[1].first = _depthImage;
		attachments[1].second = _depthImageView;

		for (size_t i = 0; i < _framebuffers.capacity(); ++i)
		{
			auto& frameData = _perFrameData[i];
			attachments[0].first = frameData.colorImage;
			attachments[0].second = frameData.colorImageView;

			_framebuffers.push_back(_imageList.AddFramebuffer(swapchainData.renderPassID, attachments, swapchainData.renderImagesWidth, swapchainData.renderImagesHeight,
				1, _framebuffers.capacity()));
		}

		{
			_gammaCorrectionDescriptorPool = _descriptorList.AddNoIndividualFreeingDescriptorPool(static_cast<uint32_t>(_perFrameData.size()),
				{ {VS::DescriptorTypeFlagBits::INPUT_ATTACHMENT, static_cast<uint32_t>(_perFrameData.size())} });

			std::vector<IDObject<VS::AutoCleanupDescriptorSetLayout>> descriptorLayoutList;
			descriptorLayoutList.resize(_perFrameData.size(), swapchainData.descriptorLayoutID);
			auto descriptorSets = _descriptorList.AllocateNIFDescriptorSets(_gammaCorrectionDescriptorPool, descriptorLayoutList);

			std::vector<VS::DescriptorSetInputAttachmentWriteData> inputAttachmentWritingLists;
			inputAttachmentWritingLists.resize(_perFrameData.size());

			for (size_t i = 0; i < _perFrameData.size(); ++i)
			{
				auto& frameData = _perFrameData[i];
				frameData.gammaCorrectionDescriptorSet = descriptorSets[i];

				inputAttachmentWritingLists[i].descriptorSetID = descriptorSets[i];
				inputAttachmentWritingLists[i].binding = 0;
				inputAttachmentWritingLists[i].startArrayIndex = 0;
				inputAttachmentWritingLists[i].imageDataList.resize(1);

				inputAttachmentWritingLists[i].imageDataList[0].first.emplace(frameData.colorImage, frameData.colorImageView);
				inputAttachmentWritingLists[i].imageDataList[0].second = VS::ImageLayoutFlags::GENERAL;
			}

			_descriptorList.WriteNIFDescriptorSetInputAttachmentBindings(_gammaCorrectionDescriptorPool, inputAttachmentWritingLists);
		}
	}

	WindowDataInternal::~WindowDataInternal()
	{
	}

	void WindowDataInternal::ChangeSwapchainData(RenderImagesData newSwapchainData)
	{
		if (_swapchainData == newSwapchainData)
			return;

		bool redoSwapchain = false;
		bool redoColorImage = false;
		bool redoDepthImage = false;
		bool redoSynchro = false;
		bool redoFramebuffer = false;

		if (_swapchainData.colorFormat != newSwapchainData.colorFormat || _swapchainData.framesInFlight != newSwapchainData.framesInFlight)
		{
			redoSwapchain = true;
			redoColorImage = true;
			redoFramebuffer = true;
			redoSynchro = _swapchainData.framesInFlight != newSwapchainData.framesInFlight;
		}

		if (_swapchainData.depthFormat != newSwapchainData.depthFormat)
		{
			redoDepthImage = true;
			redoFramebuffer = true;
		}

		if (_swapchainData.renderImagesWidth != newSwapchainData.renderImagesWidth || _swapchainData.renderImagesHeight != newSwapchainData.renderImagesHeight)
		{
			redoColorImage = true;
			redoDepthImage = true;
			redoFramebuffer = true;
		}

		if (_swapchainData.renderPassID != newSwapchainData.renderPassID)
		{
			redoFramebuffer = true;
		}

		if (redoFramebuffer)
			DeleteFramebuffers();

		if (redoSwapchain)
			RedoSwapchain(newSwapchainData.colorFormat, static_cast<uint32_t>(newSwapchainData.framesInFlight));

		if (redoSynchro || redoColorImage)
			RedoPerFrameData(newSwapchainData.colorFormat, static_cast<uint32_t>(newSwapchainData.framesInFlight),
				newSwapchainData.renderImagesWidth, newSwapchainData.renderImagesHeight);

		if (redoDepthImage)
			RedoDepthImage(newSwapchainData.depthFormat, newSwapchainData.renderImagesWidth, newSwapchainData.renderImagesHeight);

		if (redoFramebuffer)
			RedoFramebuffers(newSwapchainData.renderPassID, newSwapchainData.renderImagesWidth, newSwapchainData.renderImagesHeight);

		_swapchainData = newSwapchainData;
	}

	const PerFrameData& WindowDataInternal::GetFrameData(size_t frameIndex) const
	{
		if (frameIndex >= _perFrameData.size())
			throw std::runtime_error("WindowDataInternal::GetFrameData Error: Program tried to read past the vector data!");

		return _perFrameData[frameIndex];
	}

	IDObject<VS::AutoCleanupFramebuffer> WindowDataInternal::GetFramebufferID(size_t graphicsFrameIndice)
	{
		if (graphicsFrameIndice >= _framebuffers.size())
			throw std::runtime_error("WindowDataInternal::GetFrameData Error: Program tried to access a non-existent framebuffer!");

		return _framebuffers[graphicsFrameIndice];
	}

	uint32_t WindowDataInternal::GetRenderWidth() const
	{
		return _swapchainData.renderImagesWidth;
	}

	uint32_t WindowDataInternal::GetRenderHeight() const
	{
		return _swapchainData.renderImagesHeight;
	}

	void WindowDataInternal::ChangeFullscreen(Misc::Bool64Values newFullscreen)
	{
		if (_fullscreen == newFullscreen)
			return;

		_window->SetFullscreen(newFullscreen);
		_fullscreen = newFullscreen;
	}

	bool WindowDataInternal::RenderingShouldBePaused() const
	{
		return _window->IsMinimized() || _window->IsHidden();
	}

	void WindowDataInternal::RedoSwapchain(VS::DataFormatSetIndependentID colorFormat, uint32_t framesInFlight)
	{
		VS::SwapchainCreationData swapchainCreationData;

		swapchainCreationData.compositeAlphaMode = VS::COMPOSITE_ALPHA_OPAQUE;
		swapchainCreationData.surfaceTranformMode = VS::SURFACE_TRASFORM_IDENTITY;
		swapchainCreationData.surfacePresentMode = VS::PRESENT_MODE_FIFO_STRICT;
		swapchainCreationData.format = colorFormat;

		swapchainCreationData.imageAmount = framesInFlight;

		_window->CreateSwapchain(swapchainCreationData, false);
	}

	void WindowDataInternal::RedoPerFrameData(VS::DataFormatSetIndependentID colorFormat, uint32_t framesInFlight, uint32_t width, uint32_t height)
	{
		bool resizeFrameData = framesInFlight != _perFrameData.size();

		for (size_t i = 0; i < _perFrameData.size(); ++i)
		{
			auto& frameData = _perFrameData[i];
			_imageList.RemoveColorRenderTargetImage(frameData.colorImage, true);
		}

		_memoryList.FreeMemory(_colorImageMemory, true, true);

		if (resizeFrameData)
			_perFrameData.resize(framesInFlight);

		_descriptorList.DeleteNIFDescriptorPool(_gammaCorrectionDescriptorPool, true);
		_gammaCorrectionDescriptorPool = _descriptorList.AddNoIndividualFreeingDescriptorPool(framesInFlight,
			{ {VS::DescriptorTypeFlagBits::INPUT_ATTACHMENT, framesInFlight} });

		std::vector<IDObject<VS::AutoCleanupDescriptorSetLayout>> descriptorLayoutList;
		descriptorLayoutList.resize(framesInFlight, _swapchainData.descriptorLayoutID);
		auto descriptorSets = _descriptorList.AllocateNIFDescriptorSets(_gammaCorrectionDescriptorPool, descriptorLayoutList);

		for (size_t i = 0; i < _perFrameData.size(); ++i)
		{
			auto& frameData = _perFrameData[i];
			frameData.colorImage = _imageList.AddColorRenderTargetImage(width, height, colorFormat,
				VS::SAMPLE_1, {}, false, false, 1, static_cast<size_t>(framesInFlight) + 1);

			frameData.gammaCorrectionDescriptorSet = descriptorSets[i];
		}

		_colorImageMemory = _memoryList.AllocateMemory(_imageList.GetColorRenderTargetImagesSize(_perFrameData.back().colorImage) * _perFrameData.size(), _perFrameData.size(),
			_colorMemoryProperties, _imageList.GetColorRenderTargetImagesMemoryTypeMask(_perFrameData.back().colorImage), 0x10);

		std::vector<VS::DescriptorSetInputAttachmentWriteData> inputAttachmentWritingLists;
		inputAttachmentWritingLists.resize(_perFrameData.size());

		for (size_t i = 0; i < _perFrameData.size(); ++i)
		{
			auto& frameData = _perFrameData[i];

			_imageList.BindColorRenderTargetImage(frameData.colorImage, _colorImageMemory, static_cast<size_t>(framesInFlight) + 1);
			frameData.colorImageView = _imageList.AddColorRenderTargetImageView(frameData.colorImage);

			inputAttachmentWritingLists[i].descriptorSetID = descriptorSets[i];
			inputAttachmentWritingLists[i].binding = 0;
			inputAttachmentWritingLists[i].startArrayIndex = 0;
			inputAttachmentWritingLists[i].imageDataList.resize(1);

			inputAttachmentWritingLists[i].imageDataList[0].first.emplace(frameData.colorImage, frameData.colorImageView);
			inputAttachmentWritingLists[i].imageDataList[0].second = VS::ImageLayoutFlags::GENERAL;
		}

		_descriptorList.WriteNIFDescriptorSetInputAttachmentBindings(_gammaCorrectionDescriptorPool, inputAttachmentWritingLists);
	}

	void WindowDataInternal::RedoDepthImage(VS::DataFormatSetIndependentID depthFormat, uint32_t width, uint32_t height)
	{
		_imageList.RemoveDepthStencilRenderTargetImage(_depthImage, true);
		_memoryList.FreeMemory(_depthImageMemory, true, true);

		_depthImage = _imageList.AddDepthStencilRenderTargetImage(width, height, depthFormat, VS::SAMPLE_1, {}, false, true, 1);
		_depthImageMemory = _memoryList.AllocateMemory(_imageList.GetDepthStencilRenderTargetImagesSize(_depthImage), 1, _depthMemoryProperties,
			_imageList.GetDepthStencilRenderTargetImagesMemoryTypeMask(_depthImage), 0x10);
		_imageList.BindDepthStencilRenderTargetImage(_depthImage, _depthImageMemory);
		_depthImageView = _imageList.AddDepthStencilRenderTargetImageView(_depthImage);
	}

	void WindowDataInternal::DeleteFramebuffers()
	{
		for (size_t i = 0; _framebuffers.size(); ++i)
		{
			_imageList.RemoveFramebuffer(_framebuffers[i], true);
		}

		_framebuffers.clear();
	}

	void WindowDataInternal::RedoFramebuffers(IDObject<VS::AutoCleanupRenderPass> renderPassID, uint32_t width, uint32_t height)
	{
		_framebuffers.resize(_perFrameData.size());

		std::vector<std::pair<VS::RenderTargetImagesID, IDObject<VS::AutoCleanupImageView>>> attachments;
		attachments.resize(2);
		attachments[1].first = _depthImage;
		attachments[1].second = _depthImageView;

		for (size_t i = 0; i < _framebuffers.size(); ++i)
		{
			auto& frameData = _perFrameData[i];
			attachments[0].first = frameData.colorImage;
			attachments[0].second = frameData.colorImageView;

			_framebuffers[i] = _imageList.AddFramebuffer(renderPassID, attachments, width, height, 1, _framebuffers.size());
		}
	}

}