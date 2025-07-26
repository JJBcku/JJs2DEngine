#pragma once

#include <CustomLists/IDObject.h>

#include "VSDescriptorDataListsDef.h"
#include "VSDataBufferListsDef.h"

#include <vector>
#include <optional>

namespace VulkanSimplified
{
	struct DescriptorSetUniformBufferBindingWriteData
	{
		IDObject<AutoCleanupDescriptorSet> descriptorSetID;
		uint32_t binding;
		uint32_t startArrayIndex;
		std::vector<std::optional<IDObject<AutoCleanupUniformBuffer>>> uniformBufferIDList;

		DescriptorSetUniformBufferBindingWriteData();
	};
}