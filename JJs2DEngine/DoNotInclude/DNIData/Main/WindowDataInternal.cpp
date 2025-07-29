#include "MainDNIpch.h"
#include "WindowDataInternal.h"

#include "../../../Include/Main/WindowInitializationData.h"

#include "DeviceSettingsInternal.h"

#include <Miscellaneous/Bool64.h>

#include <VulkanSimplified/VSCommon/VSImageSampleFlags.h>

#include <VulkanSimplified/VSDevice/VSWindow.h>

#include <VulkanSimplified/VSDevice/VSWindowCreationData.h>
#include <VulkanSimplified/VSDevice/VSSwapchainCreationData.h>

#include <VulkanSimplified/VSCommon/VSDataFormatFlags.h>

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

	WindowDataInternal::WindowDataInternal(const WindowInitializationData& initData, RenderImagesData swapchainData, VS::WindowList windowList, VS::SynchronizationDataLists synchroList,
		VS::ImageDataLists imageList, VS::MemoryObjectsList memoryList) : _windowList(windowList), _synchroList(synchroList), _imageList(imageList), _memoryList(memoryList)
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

		auto window = _windowList.GetWindow(_windowID);

		VS::SwapchainCreationData swapchainCreationData;

		swapchainCreationData.compositeAlphaMode = VS::COMPOSITE_ALPHA_OPAQUE;
		swapchainCreationData.surfaceTranformMode = VS::SURFACE_TRASFORM_IDENTITY;
		swapchainCreationData.surfacePresentMode = VS::PRESENT_MODE_FIFO_STRICT;
		swapchainCreationData.format = swapchainData.colorFormat;

		swapchainCreationData.imageAmount = static_cast<uint32_t>(swapchainData.framesInFlight);

		window.CreateSwapchain(swapchainCreationData, true);

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

			frameData._renderingFinishedFence = _synchroList.AddFence(_perFrameData.size() * 2);
			frameData._transferFinishedFence = _synchroList.AddFence(_perFrameData.size() * 2);

			frameData._transferFinishedSemaphore = _synchroList.AddSemaphore(_perFrameData.size() * 8);
			frameData._imageAcquiredSemaphore = _synchroList.AddSemaphore(_perFrameData.size() * 8);
			frameData._renderingFinishedSemaphore = _synchroList.AddSemaphore(_perFrameData.size() * 8);

			frameData._colorImage = _imageList.AddColorRenderTargetImage(swapchainData.renderImagesWidth, swapchainData.renderImagesHeight, swapchainData.colorFormat,
				VS::SAMPLE_1, {}, false, 1, _perFrameData.size() + 1);
		}

		_depthImage = _imageList.AddDepthStencilRenderTargetImage(swapchainData.renderImagesWidth, swapchainData.renderImagesHeight, swapchainData.depthFormat,
			VS::SAMPLE_1, {}, false, 1, _perFrameData.size() + 1);

		_colorImageMemory = _memoryList.AllocateMemory(imageList.GetColorRenderTargetImagesSize(_perFrameData.back()._colorImage) * _perFrameData.size(), _perFrameData.size(),
			_colorMemoryProperties, imageList.GetColorRenderTargetImagesMemoryTypeMask(_perFrameData.back()._colorImage), 0x10);

		_depthImageMemory = _memoryList.AllocateMemory(imageList.GetDepthStencilRenderTargetImagesSize(_depthImage), 1, _depthMemoryProperties,
			imageList.GetDepthStencilRenderTargetImagesMemoryTypeMask(_depthImage), 0x10);

		for (size_t i = 0; i < _perFrameData.size(); ++i)
		{
			auto& frameData = _perFrameData[i];
			imageList.BindColorRenderTargetImage(frameData._colorImage, _colorImageMemory);
			frameData._colorImageView = imageList.AddColorRenderTargetImageView(frameData._colorImage);
		}

		imageList.BindDepthStencilRenderTargetImage(_depthImage, _depthImageMemory);
		_depthImageView = imageList.AddDepthStencilRenderTargetImageView(_depthImage);
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

		if (_swapchainData.colorFormat != newSwapchainData.colorFormat || _swapchainData.framesInFlight != newSwapchainData.framesInFlight)
		{
			redoSwapchain = true;
			redoColorImage = true;
			redoSynchro = _swapchainData.framesInFlight != newSwapchainData.framesInFlight;
		}

		if (_swapchainData.depthFormat != newSwapchainData.depthFormat)
			redoDepthImage = true;

		if (_swapchainData.renderImagesWidth != newSwapchainData.renderImagesWidth || _swapchainData.renderImagesHeight != newSwapchainData.renderImagesHeight)
		{
			redoColorImage = true;
			redoDepthImage = true;
		}

		if (redoSwapchain)
		{
			VS::SwapchainCreationData swapchainCreationData;

			swapchainCreationData.compositeAlphaMode = VS::COMPOSITE_ALPHA_OPAQUE;
			swapchainCreationData.surfaceTranformMode = VS::SURFACE_TRASFORM_IDENTITY;
			swapchainCreationData.surfacePresentMode = VS::PRESENT_MODE_FIFO_STRICT;
			swapchainCreationData.format = newSwapchainData.colorFormat;

			swapchainCreationData.imageAmount = static_cast<uint32_t>(newSwapchainData.framesInFlight);

			auto window = _windowList.GetWindow(_windowID);
			window.CreateSwapchain(swapchainCreationData, false);
		}

		if (redoSynchro || redoColorImage)
		{
			for (size_t i = 0; i < _perFrameData.size(); ++i)
			{
				auto& frameData = _perFrameData[i];
				_imageList.RemoveColorRenderTargetImage(frameData._colorImage, true);

				if (redoSynchro)
				{
					_synchroList.RemoveFence(frameData._renderingFinishedFence, true);
					_synchroList.RemoveFence(frameData._transferFinishedFence, true);
					_synchroList.RemoveSemaphore(frameData._imageAcquiredSemaphore, true);
					_synchroList.RemoveSemaphore(frameData._renderingFinishedSemaphore, true);
					_synchroList.RemoveSemaphore(frameData._renderingFinishedSemaphore, true);
				}
			}

			_memoryList.FreeMemory(_colorImageMemory, true, true);

			if (redoSynchro)
				_perFrameData.resize(newSwapchainData.framesInFlight);

			for (size_t i = 0; i < _perFrameData.size(); ++i)
			{
				auto& frameData = _perFrameData[i];
				frameData._colorImage = _imageList.AddColorRenderTargetImage(newSwapchainData.renderImagesWidth, newSwapchainData.renderImagesWidth, newSwapchainData.colorFormat,
					VS::SAMPLE_1, {}, false, false, 1, newSwapchainData.framesInFlight + 1);

				if (redoSynchro)
				{
					frameData._renderingFinishedFence = _synchroList.AddFence(_perFrameData.size() * 2);
					frameData._transferFinishedFence = _synchroList.AddFence(_perFrameData.size() * 2);

					frameData._transferFinishedSemaphore = _synchroList.AddSemaphore(_perFrameData.size() * 8);
					frameData._imageAcquiredSemaphore = _synchroList.AddSemaphore(_perFrameData.size() * 8);
					frameData._renderingFinishedSemaphore = _synchroList.AddSemaphore(_perFrameData.size() * 8);
				}
			}

			_colorImageMemory = _memoryList.AllocateMemory(_imageList.GetColorRenderTargetImagesSize(_perFrameData.back()._colorImage) * _perFrameData.size(), _perFrameData.size(),
				_colorMemoryProperties, _imageList.GetColorRenderTargetImagesMemoryTypeMask(_perFrameData.back()._colorImage), 0x10);

			for (size_t i = 0; i < _perFrameData.size(); ++i)
			{
				_imageList.BindColorRenderTargetImage(_perFrameData[i]._colorImage, _colorImageMemory, 0x10);
				_perFrameData[i]._colorImageView = _imageList.AddColorRenderTargetImageView(_perFrameData[i]._colorImage);
			}
		}

		if (redoDepthImage)
		{
			_imageList.RemoveDepthStencilRenderTargetImage(_depthImage, true);
			_memoryList.FreeMemory(_depthImageMemory, true, true);

			_depthImage = _imageList.AddDepthStencilRenderTargetImage(newSwapchainData.renderImagesWidth, newSwapchainData.renderImagesWidth, newSwapchainData.depthFormat,
				VS::SAMPLE_1, {}, false, true, 1, newSwapchainData.framesInFlight + 1);
			_depthImageMemory = _memoryList.AllocateMemory(_imageList.GetDepthStencilRenderTargetImagesSize(_depthImage), 1, _depthMemoryProperties,
				_imageList.GetDepthStencilRenderTargetImagesMemoryTypeMask(_depthImage), 0x10);
			_imageList.BindDepthStencilRenderTargetImage(_depthImage, _depthImageMemory);
			_depthImageView = _imageList.AddDepthStencilRenderTargetImageView(_depthImage);
		}

		_swapchainData = newSwapchainData;
	}

	const PerFrameData& WindowDataInternal::GetFrameData(size_t frameIndex) const
	{
		if (frameIndex >= _perFrameData.size())
			throw std::runtime_error("WindowDataInternal::GetFrameData Error: Program tried to read past the vector data!");

		return _perFrameData[frameIndex];
	}

}