#pragma once

#include <CustomLists/IDObject.h>

#include <vector>

namespace VulkanSimplified
{
	class AutoCleanupDescriptorSetLayout;
	struct PushConstantData;
}

namespace VulkanSimplified
{
	struct PipelineLayoutCreationData
	{
		std::vector<IDObject<AutoCleanupDescriptorSetLayout>> _descriptorSets;
		std::vector<IDObject<PushConstantData>> _pushConstants;

		PipelineLayoutCreationData() = default;
	};
}