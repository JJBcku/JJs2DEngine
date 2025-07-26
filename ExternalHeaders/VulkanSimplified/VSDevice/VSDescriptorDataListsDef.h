#pragma once

#include <CustomLists/IDObjectDef.h>

#include <utility>
#include <vector>

namespace VulkanSimplified
{
	class DescriptorDataLists;

	class AutoCleanupDescriptorSetLayout;

	class AutoCleanupNIFDescriptorPool;
	class AutoCleanupIFDescriptorPool;

	class AutoCleanupDescriptorSet;

	struct DescriptorSetLayoutBindingData;
	class AutoCleanupSampler;
}

namespace VulkanSimplified
{
	typedef std::pair<IDObject<DescriptorSetLayoutBindingData>,
		std::vector<IDObject<AutoCleanupSampler>>> DescriptorSetBindingFullData;
}
