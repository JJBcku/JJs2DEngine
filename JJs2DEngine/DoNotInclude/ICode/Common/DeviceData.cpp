#include "CommonIpch.h"
#include "../../../Include/Common/DeviceData.h"

namespace JJs2DEngine
{
	DeviceQueueFamilySupport::DeviceQueueFamilySupport()
	{
		noVideoCodingGraphicQueueFamilyImageGranularity = std::numeric_limits<uint32_t>::max();
		noVideoCodingGraphicQueueFamilyTimestampGranularity = 0;

		videoCodingGraphicQueueFamilyImageGranularity = std::numeric_limits<uint32_t>::max();
		videoCodingGraphicQueueFamilyTimestampGranularity = 0;

		computeQueueFamilyImageGranularity = std::numeric_limits<uint32_t>::max();
		computeQueueFamilyTimestampGranularity = 0;

		transferQueueFamilyImageGranularity = std::numeric_limits<uint32_t>::max();
		transferQueueFamilyTimestampGranularity = 0;
	}

	bool DeviceQueueFamilySupport::HasAnyGraphicsQueue() const
	{
		return noVideoCodingGraphicQueueFamily.has_value() || videoCodingGraphicQueueFamily.has_value();
	}

	DeviceSwapchainSupport::DeviceSwapchainSupport()
	{
		minFramesInFlight = 0;
		maxFramesInFlight = 0;

		swapchainRGBA16Unorm = false;

		swapchainA2RGB10Unorm = false;
		swapchainA2BGR10Unorm = false;

		swapchainRGBA8Unorm = false;

		swapchainBGRA8Unorm = false;

		swapchainABGR8Unorm = false;
	}

	bool DeviceSwapchainSupport::Support8BitSwapchain() const
	{
		return swapchainRGBA8Unorm ||  swapchainBGRA8Unorm || swapchainABGR8Unorm;
	}

	bool DeviceSwapchainSupport::Support10BitSwapchain() const
	{
		return swapchainA2RGB10Unorm || swapchainA2BGR10Unorm;
	}

	bool DeviceSwapchainSupport::Support16BitSwapchain() const
	{
		return swapchainRGBA16Unorm;
	}

	bool DeviceSwapchainSupport::SupportAnySwapchainFormatSize() const
	{
		return Support8BitSwapchain() || Support10BitSwapchain() || Support16BitSwapchain();
	}

	DeviceTextureSupport::DeviceTextureSupport()
	{
		textureRGBA16UNORM = false;

		textureRGBA8UNORM = false;
		textureBGRA8UNORM = false;

		textureBC = false;
		textureETC2 = false;
		textureASTC = false;
	}

	bool DeviceTextureSupport::SupportAnyUncompressedTextures() const
	{
		return textureRGBA16UNORM || textureRGBA8UNORM || textureBGRA8UNORM;
	}

	bool DeviceTextureSupport::Support16BitUncompressedTextures() const
	{
		return textureRGBA16UNORM;
	}

	DeviceDepthStencilSupport::DeviceDepthStencilSupport()
	{
		D32Float = false;
		D32FloatS8Int = false;
	}

	bool DeviceDepthStencilSupport::SupportAnyDepthFormat() const
	{
		return D32Float || D32FloatS8Int;
	}

	DeviceData::DeviceData()
	{
		deviceIndex = std::numeric_limits<uint64_t>::max();
		deviceLocalMemory = 0;
		lazilyAllocatedMemory = 0;
		sharedMemory = 0;
	}

}