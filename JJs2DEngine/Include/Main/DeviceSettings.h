#pragma once

#include <stdint.h>

#include "PipelineListInitializationData.h"

namespace JJs2DEngine
{
	enum class AspectRatio : uint32_t
	{
		UNSET = 0,
		ASPECT_RATIO_16_9 = 1,
	};

	enum class SwapchainFormat : uint64_t
	{
		SWAPCHAIN_FORMAT_UNSET = 0,
		SWAPCHAIN_FORMAT_RGB16 = 1,
		SWAPCHAIN_FORMAT_RGBA16 = 2,

		SWAPCHAIN_FORMAT_A2RGB10 = 4,
		SWAPCHAIN_FORMAT_A2BGR10 = 8,

		SWAPCHAIN_FORMAT_RGB8 = 0x10,
		SWAPCHAIN_FORMAT_RGBA8 = 0x20,

		SWAPCHAIN_FORMAT_BGR8 = 0x40,
		SWAPCHAIN_FORMAT_BGRA8 = 0x80,

		SWAPCHAIN_FORMAT_ABGR8 = 0x100,
	};

	enum class TextureFormat : uint64_t
	{
		TEXTURE_FORMAT_UNSET = 0,
		TEXTURE_FORMAT_RGBA16 = 1,

		TEXTURE_FORMAT_RGBA8 = 2,
		TEXTURE_FORMAT_BGRA8 = 4,
	};

	enum class DepthFormat : uint64_t
	{
		DEPTH_FORMAT_UNSET = 0,
		DEPTH_FORMAT_D32 = 1,
		DEPTH_FORMAT_D32_S8 = 2,
	};

	struct DeviceSettings
	{
		size_t deviceIndex;

		uint32_t windowWidth;
		AspectRatio aspectRatio;

		uint64_t framesInFlight;

		SwapchainFormat swapchainFormat;
		TextureFormat textureFormat;
		DepthFormat depthFormat;

		PipelineListInitializationData pipelineListInitData;

		DeviceSettings();
	};
}
