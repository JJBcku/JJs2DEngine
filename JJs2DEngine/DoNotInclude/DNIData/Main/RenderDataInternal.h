#pragma once

#include <optional>

#include <CustomLists/IDObject.h>

#include <VulkanSimplified/VSDevice/VSDeviceMain.h>
#include <VulkanSimplified/VSDevice/VSPipelineDataListsDef.h>
#include <VulkanSimplified/VSDevice/VSDescriptorDataListDef.h>
#include <VulkanSimplified/VSDevice/VSRenderPassDataListDef.h>

#include <VulkanSimplified/VSSharedData/VSSharedDataMainList.h>
#include <VulkanSimplified/VSSharedData/VSSharedRenderPassDataListDef.h>
#include <VulkanSimplified/VSSharedData/VSRenderPassClearValueID.h>

#include <VulkanSimplified/VSDevice/VSGraphicsPipelineCreationData.h>

namespace JJs2DEngine
{
	struct PipelineSettings;

	class RenderDataInternal
	{
	public:
		RenderDataInternal(size_t currentPipelineSettings, const std::vector<PipelineSettings>& preInitializedPipelineSettings, const std::string& dataFolder,
			VS::DeviceMain device, VS::SharedDataMainList sharedData);
		~RenderDataInternal();

		IDObject<VS::AutoCleanupRenderPass> GetCurrentRenderPass() const;

	private:
		VS::DeviceMain _device;
		VS::SharedDataMainList _sharedData;

		std::vector<std::optional<VS::RenderPassClearValueID>> clearValues;

		size_t _currentPipelineSettings;

		std::optional<uint64_t> _uiPipelineCompatibleSavedPos;
		std::optional<uint64_t> _uiPipelineCompatibleSavedSize;
		std::optional<uint64_t> _uiPipelineCompatibleSavedCRC64WE;

		IDObject<VS::AutoCleanupFragmentShaderModule> _standardFragmentShaderID;

		std::vector<IDObject<VS::AutoCleanupRenderPass>> _renderPassList;

		IDObject<VS::AutoCleanupPipelineCache> _uiPipelineCache;
		IDObject<VS::AutoCleanupPipelineCache> _gammaCorrectionPipelineCache;

		IDObject<VS::AutoCleanupVertexShaderModule> _uiVertexShaderID;
		IDObject<VS::AutoCleanupDescriptorSetLayout> _uiDescriptorSetLayout;
		IDObject<VS::AutoCleanupPipelineLayout> _uiPipelineLayout;

		std::vector<IDObject<VS::AutoCleanupGraphicsPipeline>> _uiPipelineList;

		std::optional<uint64_t> _gammaCorrectionPipelineCompatibleSavedPos;
		std::optional<uint64_t> _gammaCorrectionPipelineCompatibleSavedSize;
		std::optional<uint64_t> _gammaCorrectionPipelineCompatibleSavedCRC64WE;

		IDObject<VS::AutoCleanupFragmentShaderModule> _gammaCorrectionFragmentShaderID;
		IDObject<VS::AutoCleanupVertexShaderModule> _gammaCorrectionVertexShaderID;
		IDObject<VS::AutoCleanupDescriptorSetLayout> _gammaCorrectionDescriptorSetLayout;
		IDObject<VS::AutoCleanupPipelineLayout> _gammaCorrectionPipelineLayout;
		std::vector<IDObject<VS::AutoCleanupGraphicsPipeline>> _gammaCorrectionPipelineList;

		void CreateUIPipelineCacheFile(const std::string& dataFolder);
		void LoadUIPipelineCacheFile(const std::string& dataFolder);
		void SaveUIPipelineCacheFile(const std::string& dataFolder);

		void CreateGammaCorrectionPipelineCacheFile(const std::string& dataFolder);
		void LoadGammaCorrectionPipelineCacheFile(const std::string& dataFolder);
		void SaveGammaCorrectionPipelineCacheFile(const std::string& dataFolder);

		std::vector<char> LoadShaderFile(std::string shaderName);

		IDObject<VS::AutoCleanupRenderPass> CreateRenderPass(const PipelineSettings& pipelineSettings);

		VS::GraphicsPipelineCreationData CompileUIPipelinesCreationData(uint32_t width, uint32_t height, IDObject<VS::AutoCleanupRenderPass> renderPass);
		VS::GraphicsPipelineCreationData CompileGammaCorrectionPipelinesCreationData(uint32_t width, uint32_t height, IDObject<VS::AutoCleanupRenderPass> renderPass);
	};
}