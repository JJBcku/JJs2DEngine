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
		depthFormat = DepthFormat::DEPTH_FORMAT_UNSET;
	}

	DeviceSettings::DeviceSettings()
	{
		deviceIndex = std::numeric_limits<size_t>::max();

		transferFramesInFlight = 0;
		graphicsFramesInFlight = 0;

		textureFormat = TextureFormat::TEXTURE_FORMAT_UNSET;
		std::memset(preLoadedTexturesMaxAmounts.data(), 0, sizeof(preLoadedTexturesMaxAmounts[0]) * preLoadedTexturesMaxAmounts.size());
		std::memset(streamedTexturesMaxAmounts.data(), 0, sizeof(streamedTexturesMaxAmounts[0]) * streamedTexturesMaxAmounts.size());

		preLoadedTexturesStagingBuffersPageCount = 0;
		streamedTexturesStagingBuffersPageCount = 0;

		currentPipelineSettings = std::numeric_limits<size_t>::max();
	}

}