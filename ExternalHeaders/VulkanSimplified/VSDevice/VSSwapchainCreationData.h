#pragma once

#include "../VSCommon/VSCompositeAlphaFlags.h"
#include "../VSCommon/VSSurfacePresentModes.h"
#include "../VSCommon/VSSurfaceTransformFlags.h"
#include "../VSCommon/VSDataFormatFlags.h"

#include <vector>
#include <stdint.h>

namespace VulkanSimplified
{
	struct SwapchainCreationData
	{
		CompositeAlphaFlagBits compositeAlphaMode;
		SurfaceTransformFlagBits surfaceTranformMode;
		SurfacePresentModeBits surfacePresentMode;
		DataFormatSetIndependentID format;

		uint32_t imageAmount;
		uint32_t padding;

		std::vector<size_t> queuesUsingSwapchain;

		SwapchainCreationData();
	};
}
