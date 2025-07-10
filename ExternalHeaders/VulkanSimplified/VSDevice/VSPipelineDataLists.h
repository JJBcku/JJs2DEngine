#pragma once

#include <CustomLists\IDObject.h>

#include "VSPipelineDataListsDef.h"

#include <vector>
#include <optional>

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

		std::optional<IDObject<AutoCleanupPipelineCache>> AddPipelineCache(const std::vector<unsigned char>& initialData = {}, size_t addOnReserving = 0);

		IDObject<AutoCleanupPipelineLayout> AddPipelineLayout(const PipelineLayoutCreationData& creationData, size_t addOnReserving = 0);
		std::vector<IDObject<AutoCleanupGraphicsPipeline>> AddGraphicPipelines(const std::vector<GraphicsPipelineCreationData>& creationDataList,
			std::optional<IDObject<AutoCleanupPipelineCache>> pipelineCacheID = {}, size_t addOnReserving = 0);

		size_t GetPipelineCacheSize(IDObject<AutoCleanupPipelineCache> cacheID) const;
		std::vector<unsigned char> GetPipelineCacheData(IDObject<AutoCleanupPipelineCache> cacheID) const;

	private:
		PipelineDataListsInternal& _internal;
	};
}