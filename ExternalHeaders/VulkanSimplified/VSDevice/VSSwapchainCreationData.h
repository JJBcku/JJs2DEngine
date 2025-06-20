#pragma once

#include "../VSCommon/VSSurfacePresentModes.h"
#include "../VSCommon/VSDataFormatFlags.h"

#include <vector>
#include <stdint.h>

namespace VulkanSimplified
{
	struct SwapchainCreationData
	{
		SurfacePresentModeBits surfacePresentMode;
		DataFormatSetIndependentID format;

		uint32_t imageAmount;
		uint32_t padding;

		std::vector<size_t> queuesUsingSwapchain;

		SwapchainCreationData();
	};
}
