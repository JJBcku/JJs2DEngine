#pragma once

#include "VSSharedDescriptorDataListCapacities.h"
#include "VSSharedRenderPassDataListsCapacities.h"
#include "VSSharedPipelineDataListsCapacities.h"

namespace VulkanSimplified
{
	struct SharedDataListsCapacities
	{
		SharedDescriptorDataListCapacities descriptorLists;
		SharedRenderPassDataListsCapacities renderPassLists;
		SharedPipelineDataListsssCapacities pipelineLists;

		SharedDataListsCapacities() = default;
	};
}