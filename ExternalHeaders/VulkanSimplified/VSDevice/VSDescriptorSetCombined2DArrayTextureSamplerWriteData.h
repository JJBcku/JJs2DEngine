#pragma once

#include "VSDescriptorDataListsDef.h"
#include "VSImageDataListsDef.h"
#include "VSOptionalSamplerDef.h"

#include "../VSCommon/VSImageLayoutFlags.h"

#include <CustomLists/IDObject.h>

#include <optional>

namespace VulkanSimplified
{
	typedef std::optional<std::pair<IDObject<AutoCleanup2DArrayTexture>, IDObject<AutoCleanupImageView>>> Optional2DArrayTextureView;
	typedef std::pair<Optional2DArrayTextureView, OptionalSampler> Combined2DArrayTextureSamplerIDs;

	struct DescriptorSetCombined2DArrayTextureSamplerWriteData
	{
		IDObject<AutoCleanupDescriptorSet> descriptorSetID;
		uint32_t binding;
		uint32_t startArrayIndex;
		std::vector<std::pair<Combined2DArrayTextureSamplerIDs, ImageLayoutFlags>> imageDataList;

		DescriptorSetCombined2DArrayTextureSamplerWriteData();
	};
}