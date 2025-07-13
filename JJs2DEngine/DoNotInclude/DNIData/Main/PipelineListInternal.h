#pragma once

#include <optional>

#include <CustomLists/IDObject.h>

#include <VulkanSimplified/VSDevice/VSPipelineDataLists.h>

namespace JJs2DEngine
{
	struct PipelineSettings;

	class PipelineListInternal
	{
	public:
		PipelineListInternal(const PipelineSettings& currentPipelineSettings, const std::vector<PipelineSettings>& preInitializedPipelineSettings,
			VS::PipelineDataLists VSpipelineDataList);
		~PipelineListInternal();

	private:
		VS::PipelineDataLists _VSpipelineDataList;

		std::optional<uint64_t> _uiPipelineCompatibleSavedPos;
		std::optional<uint64_t> _uiPipelineCompatibleSavedSize;
		
		IDObject<VS::AutoCleanupPipelineCache> _uiPipelineCache;

		void CreateUIPipelineCacheFile();
		void LoadUIPipelineCacheFile();

	};
}