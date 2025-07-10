#pragma once

namespace VulkanSimplified
{
	struct PipelineDataListsInitialCapacities
	{
		size_t pipelineCacheListInitialCapacity;

		size_t pipelineLayoutListInitialCapacity;
		size_t graphicsPipelineListInitialCapacity;

		PipelineDataListsInitialCapacities();
	};
}