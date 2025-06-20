#pragma once

#include <CustomLists\IDObjectDef.h>

#include "VSPipelineDataListsDef.h"

#include <vector>

namespace VulkanSimplified
{
	class PipelineDataListsInternal;
}

namespace VulkanSimplified
{
	struct PipelineLayoutCreationData;
	struct GraphicsPipelineCreationData;

	class PipelineDataLists
	{
	public:
		PipelineDataLists(PipelineDataListsInternal& ref);
		~PipelineDataLists();

		IDObject<AutoCleanupPipelineLayout> AddPipelineLayout(const PipelineLayoutCreationData& creationData, size_t addOnReserving = 0);
		std::vector<IDObject<AutoCleanupGraphicsPipeline>> AddGraphicPipelines(const std::vector<GraphicsPipelineCreationData>& creationDataList,
			size_t addOnReserving = 0);

	private:
		PipelineDataListsInternal& _internal;
	};
}