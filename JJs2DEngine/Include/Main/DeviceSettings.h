#pragma once

#include "AspectRatioDef.h"
#include "WindowInitializationData.h"

#include <stdint.h>
#include <vector>
#include <string>
#include <compare>

namespace JJs2DEngine
{
	enum class SwapchainFormat : uint64_t
	{
		SWAPCHAIN_FORMAT_UNSET = 0x0,
		SWAPCHAIN_FORMAT_RGBA16 = 0x1,

		SWAPCHAIN_FORMAT_A2RGB10 = 0x2,
		SWAPCHAIN_FORMAT_A2BGR10 = 0x4,

		SWAPCHAIN_FORMAT_RGBA8 = 0x8,
		SWAPCHAIN_FORMAT_BGRA8 = 0x10,
		SWAPCHAIN_FORMAT_ABGR8 = 0x20,
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

	struct PipelineSettings
	{
		uint32_t renderWidth;
		AspectRatio aspectRatio;

		SwapchainFormat swapchainFormat;
		DepthFormat depthFormat;

		PipelineSettings();

		std::strong_ordering operator<=>(const PipelineSettings& rhs) const noexcept = default;
		bool operator==(const PipelineSettings& rhs) const noexcept = default;
	};

	struct DeviceSettings
	{
		size_t deviceIndex;

		uint32_t transferFramesInFlight;
		uint32_t graphicsFramesInFlight;

		TextureFormat textureFormat;

		WindowInitializationData windowData;

		PipelineSettings currentPipelineSettings;

		std::vector<PipelineSettings> preInitializedPipelineSettings;

		DeviceSettings();

		std::strong_ordering operator<=>(const DeviceSettings& rhs) const noexcept = default;
		bool operator==(const DeviceSettings& rhs) const noexcept = default;
	};
}
