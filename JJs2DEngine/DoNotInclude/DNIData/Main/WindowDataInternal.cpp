#include "MainDNIpch.h"
#include "WindowDataInternal.h"

#include "../../../Include/Main/WindowInitializationData.h"

#include "DeviceSettingsInternal.h"

#include <Miscellaneous/Bool64.h>

#include <VulkanSimplified/VSDevice/VSWindowList.h>
#include <VulkanSimplified/VSDevice/VSWindow.h>

#include <VulkanSimplified/VSDevice/VSWindowCreationData.h>
#include <VulkanSimplified/VSDevice/VSSwapchainCreationData.h>

#include <VulkanSimplified/VSCommon/VSDataFormatFlags.h>

namespace JJs2DEngine
{
	WindowDataInternal::WindowDataInternal(const WindowInitializationData& initData, uint32_t framesInFlight, VS::DataFormatSetIndependentID format, VS::WindowList& windowList) :
		_windowList(windowList)
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
		swapchainCreationData.format = format;

		swapchainCreationData.imageAmount = framesInFlight;

		window.CreateSwapchain(swapchainCreationData, true);

		_framesInFlight = framesInFlight;
		_format = format;
	}

	WindowDataInternal::~WindowDataInternal()
	{
	}

	void WindowDataInternal::ChangeSwapchainFormat(VS::DataFormatSetIndependentID newFormat)
	{
		if (_format == newFormat)
			return;

		VS::SwapchainCreationData swapchainCreationData;

		swapchainCreationData.compositeAlphaMode = VS::COMPOSITE_ALPHA_OPAQUE;
		swapchainCreationData.surfaceTranformMode = VS::SURFACE_TRASFORM_IDENTITY;
		swapchainCreationData.surfacePresentMode = VS::PRESENT_MODE_FIFO_STRICT;
		swapchainCreationData.format = newFormat;

		swapchainCreationData.imageAmount = static_cast<uint32_t>(_framesInFlight);

		auto window = _windowList.GetWindow(_windowID);
		window.CreateSwapchain(swapchainCreationData, false);

		_format = newFormat;
	}

}