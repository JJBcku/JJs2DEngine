#pragma once

#include <stdint.h>

namespace VulkanSimplified
{
	struct Vulkan10MaxImageSizeLimits
	{
		uint64_t maxImageDimension1D;
		uint64_t maxImageDimension2D;
		uint64_t maxImageDimension3D;
		uint64_t maxImageArrayLayers;

		Vulkan10MaxImageSizeLimits();
	};

	struct Vulkan10DescriptorSetBindingLimits
	{
		uint64_t maxBoundDescriptorSets;
		uint64_t maxPerStageDescriptorSamplers;
		uint64_t maxPerStageDescriptorUniformBuffers;
		uint64_t maxPerStageDescriptorStorageBuffers;
		uint64_t maxPerStageDescriptorSampledImages;
		uint64_t maxPerStageDescriptorStorageImages;
		uint64_t maxPerStageDescriptorInputAttachments;
		uint64_t maxPerStageResources;

		Vulkan10DescriptorSetBindingLimits();
	};

	struct Vulkan10DescriptorSetLimits
	{
		Vulkan10DescriptorSetBindingLimits descriptorSetBindingsLimit;

		uint64_t maxDescriptorSetSamplers;
		uint64_t maxDescriptorSetUniformBuffers;
		uint64_t maxDescriptorSetUniformBuffersDynamic;
		uint64_t maxDescriptorSetStorageBuffers;
		uint64_t maxDescriptorSetStorageBuffersDynamic;
		uint64_t maxDescriptorSetSampledImages;
		uint64_t maxDescriptorSetStorageImages;
		uint64_t maxDescriptorSetInputAttachments;

		Vulkan10DescriptorSetLimits();
	};

	struct Vulkan10VertexShaderLimits
	{
		uint64_t maxVertexInputAttributes;
		uint64_t maxVertexInputBindings;
		uint64_t maxVertexInputAttributeOffset;
		uint64_t maxVertexInputBindingStride;
		uint64_t maxVertexOutputComponents;

		Vulkan10VertexShaderLimits();
	};

	struct Vulkan10FragmentShaderLimits
	{
		uint64_t maxFragmentInputComponents;
		uint64_t maxFragmentOutputAttachments;
		uint64_t maxFragmentDualSrcAttachments;
		uint64_t maxFragmentCombinedOutputResources;

		Vulkan10FragmentShaderLimits();
	};

	struct Vulkan10ViewportLimits
	{
		uint64_t maxViewports;
		uint64_t maxViewportDimensionsX;
		uint64_t maxViewportDimensionsY;
		float viewportBoundsRangeMin;
		float viewportBoundsRangeMax;
		uint64_t viewportSubPixelBits;

		Vulkan10ViewportLimits();
	};

	struct Vulkan10FramebufferLimits
	{
		uint64_t maxFramebufferWidth;
		uint64_t maxFramebufferHeight;
		uint64_t maxFramebufferLayers;
		uint64_t framebufferColorSampleCounts;
		uint64_t framebufferDepthSampleCounts;
		uint64_t framebufferStencilSampleCounts;
		uint64_t framebufferNoAttachmentsSampleCounts;

		Vulkan10FramebufferLimits();
	};

	struct Vulkan10SamplingLimits
	{
		uint64_t sampledImageColorSampleCounts;
		uint64_t sampledImageIntegerSampleCounts;
		uint64_t sampledImageDepthSampleCounts;
		uint64_t sampledImageStencilSampleCounts;
		uint64_t storageImageSampleCounts;

		Vulkan10SamplingLimits();
	};

	struct DeviceVulkan10Limits
	{
		Vulkan10MaxImageSizeLimits maxImageSizes;

		uint64_t maxUniformBufferRange;
		uint64_t maxStorageBufferRange;
		uint64_t maxPushConstantsSize;

		uint64_t maxMemoryAllocationCount;
		uint64_t maxSamplerAllocationCount;

		Vulkan10DescriptorSetLimits descriptorSetLimits;

		Vulkan10VertexShaderLimits vertexShaderLimit;
		Vulkan10FragmentShaderLimits fragmentShaderLimit;

		uint64_t subPixelPrecisionBits;
		uint64_t mipmapPrecisionBits;

		uint64_t maxDrawIndexedIndexValue;

		float maxSamplerLodBias;
		float maxSamplerAnisotropy;

		Vulkan10ViewportLimits viewportLimits;
		Vulkan10FramebufferLimits framebufferLimits;

		uint64_t maxColorAttachments;

		Vulkan10SamplingLimits samplingLimits;

		DeviceVulkan10Limits();
	};
}