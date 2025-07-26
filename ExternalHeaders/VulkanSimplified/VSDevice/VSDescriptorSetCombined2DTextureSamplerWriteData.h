#pragma once

#include <CustomLists/IDObject.h>

#include "VSDescriptorDataListsDef.h"
#include "VSImageDataListsDef.h"

#include "../VSCommon/VSImageLayoutFlags.h"

#include <optional>
#include <utility>
#include <vector>

namespace VulkanSimplified
{
	typedef std::optional<std::pair<IDObject<AutoCleanup2DTexture>, IDObject<AutoCleanupImageView>>> Optional2DTextureView;
	typedef std::optional<IDObject<AutoCleanupSampler>> OptionalSampler;
	typedef std::pair<Optional2DTextureView, OptionalSampler> Combined2DTextureSamplerIDs;

	struct DescriptorSetCombined2DTextureSamplerWriteData
	{
		IDObject<AutoCleanupDescriptorSet> descriptorSetID;
		uint32_t binding;
		uint32_t startArrayIndex;
		std::vector<std::pair<Combined2DTextureSamplerIDs, ImageLayoutFlags>> imageDataList;

		DescriptorSetCombined2DTextureSamplerWriteData();
	};
}
