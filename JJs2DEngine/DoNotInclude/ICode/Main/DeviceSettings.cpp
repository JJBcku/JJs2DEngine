#include "MainIpch.h"
#include "../../../Include/Main/DeviceSettings.h"

namespace JJs2DEngine
{
	DeviceSettings::DeviceSettings()
	{
		deviceIndex = std::numeric_limits<size_t>::max();

		windowWidth = 0;
		aspectRatio = AspectRatio::UNSET;

		framesInFlight = 0;

		swapchainFormat = SwapchainFormat::SWAPCHAIN_FORMAT_UNSET;
		textureFormat = TextureFormat::TEXTURE_FORMAT_UNSET;
		depthFormat = DepthFormat::DEPTH_FORMAT_UNSET;
	}

}