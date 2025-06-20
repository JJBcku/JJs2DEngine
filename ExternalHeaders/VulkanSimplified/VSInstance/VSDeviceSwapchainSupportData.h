#pragma once

#include "../VSCommon/VSImageUsageFlagsDef.h"
#include "../VSCommon/VSDataFormatFlags.h"
#include "../VSCommon/VSSurfacePresentModesDef.h"

#include <stdint.h>

namespace VulkanSimplified
{
	struct SurfaceSupportedColorspaceFormatsLists
	{
		DataFormatFullSetList srgbNonlinearColorspace;

		SurfaceSupportedColorspaceFormatsLists();
	};

	struct SurfaceSupportData
	{
		uint32_t minWidth;
		uint32_t minHeight;

		uint32_t maxWidth;
		uint32_t maxHeight;

		uint32_t minImageCount;
		uint32_t maxImageCount;

		ImageUsageFlags surfaceUsageFlags;
		SurfacePresentModes surfacePresentModes;
		SurfaceSupportedColorspaceFormatsLists surfaceSupportedSwapchainFormats;

		SurfaceSupportData();
	};
}
