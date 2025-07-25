#include "MainIpch.h"
#include "../../../Include/Main/DeviceSettings.h"

#include "../../../Include/Main/AspectRatio.h"

namespace JJs2DEngine
{
	PipelineSettings::PipelineSettings()
	{
		renderWidth = 0;
		aspectRatio = AspectRatio::UNSET;

		swapchainFormat = SwapchainFormat::SWAPCHAIN_FORMAT_UNSET;
		textureFormat = TextureFormat::TEXTURE_FORMAT_UNSET;
		depthFormat = DepthFormat::DEPTH_FORMAT_UNSET;
	}

	DeviceSettings::DeviceSettings()
	{
		deviceIndex = std::numeric_limits<size_t>::max();

		framesInFlight = 0;

		currentPipelineSettings = std::numeric_limits<size_t>::max();
	}

}