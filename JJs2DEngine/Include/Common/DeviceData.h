#pragma once

#include <stdint.h>
#include <compare>
#include <optional>

#include "VersionData.h"

namespace JJs2DEngine
{
	struct DeviceQueueFamilySupport
	{
		std::optional<uint32_t> noVideoCodingGraphicQueueFamily;
		uint32_t noVideoCodingGraphicQueueFamilyImageGranularity;
		uint32_t noVideoCodingGraphicQueueFamilyTimestampGranularity;

		std::optional<uint32_t> videoCodingGraphicQueueFamily;
		uint32_t videoCodingGraphicQueueFamilyImageGranularity;
		uint32_t videoCodingGraphicQueueFamilyTimestampGranularity;

		std::optional<uint32_t> computeQueueFamily;
		uint32_t computeQueueFamilyImageGranularity;
		uint32_t computeQueueFamilyTimestampGranularity;

		std::optional<uint32_t> transferQueueFamily;
		uint32_t transferQueueFamilyImageGranularity;
		uint32_t transferQueueFamilyTimestampGranularity;

		DeviceQueueFamilySupport();

		bool HasAnyGraphicsQueue() const;

		std::strong_ordering operator<=>(const DeviceQueueFamilySupport& rhs) const noexcept = default;
		bool operator==(const DeviceQueueFamilySupport& rhs) const noexcept = default;
	};

	struct DeviceSwapchainSupport
	{
		uint32_t minFramesInFlight;
		uint32_t maxFramesInFlight;

		bool swapchainRGBA16Unorm;

		bool swapchainA2RGB10Unorm;
		bool swapchainA2BGR10Unorm;

		bool swapchainRGBA8Unorm;
		bool swapchainBGRA8Unorm;
		bool swapchainABGR8Unorm;

		DeviceSwapchainSupport();
		
		bool Support8BitSwapchain() const;
		bool Support10BitSwapchain() const;
		bool Support16BitSwapchain() const;

		bool SupportAnySwapchainFormatSize() const;

		std::strong_ordering operator<=>(const DeviceSwapchainSupport& rhs) const noexcept = default;
		bool operator==(const DeviceSwapchainSupport& rhs) const noexcept = default;
	};

	struct DeviceTextureSupport
	{
		bool textureRGBA16UNORM;

		bool textureRGBA8UNORM;
		bool textureBGRA8UNORM;

		bool textureBC;
		bool textureETC2;
		bool textureASTC;

		DeviceTextureSupport();

		bool SupportAnyUncompressedTextures() const;

		bool Support16BitUncompressedTextures() const;

		std::strong_ordering operator<=>(const DeviceTextureSupport& rhs) const noexcept = default;
		bool operator==(const DeviceTextureSupport& rhs) const noexcept = default;
	};

	struct DeviceDepthStencilSupport
	{
		bool D32Float;
		bool D32FloatS8Int;

		DeviceDepthStencilSupport();

		bool SupportAnyDepthFormat() const;

		std::strong_ordering operator<=>(const DeviceDepthStencilSupport& rhs) const noexcept = default;
		bool operator==(const DeviceDepthStencilSupport& rhs) const noexcept = default;
	};

	struct DeviceData
	{
		uint64_t deviceIndex;
		uint64_t deviceLocalMemory;
		uint64_t lazilyAllocatedMemory;
		uint64_t sharedMemory;
		VersionData maxVulkanVersion;

		DeviceQueueFamilySupport queueSupport;

		DeviceSwapchainSupport swapchainSupport;
		DeviceTextureSupport textureSupport;
		DeviceDepthStencilSupport depthStencilSupport;

		DeviceData();

		std::strong_ordering operator<=>(const DeviceData& rhs) const noexcept = default;
		bool operator==(const DeviceData& rhs) const noexcept = default;
	};
}
