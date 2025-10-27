#pragma once

#include <optional>

#include <CustomLists/IDObject.h>

#include <VulkanSimplified/VSDevice/VSDeviceMain.h>
#include <VulkanSimplified/VSDevice/VSPipelineDataListsDef.h>
#include <VulkanSimplified/VSDevice/VSDescriptorDataListsDef.h>
#include <VulkanSimplified/VSDevice/VSRenderPassDataListDef.h>

#include <VulkanSimplified/VSSharedData/VSSharedDataMainList.h>
#include <VulkanSimplified/VSSharedData/VSSharedRenderPassDataListsDef.h>
#include <VulkanSimplified/VSSharedData/VSRenderPassClearValueID.h>
#include <VulkanSimplified/VSSharedData/VSSharedPipelineDataListsDef.h>

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

		float GetCurrentAspectRatio() const;

		IDObject<VS::AutoCleanupRenderPass> GetCurrentRenderPass() const;
		IDObject<VS::AutoCleanupDescriptorSetLayout> GetTextureDescriptorSetLayout() const;
		IDObject<VS::AutoCleanupDescriptorSetLayout> GetGammaCorrectionDescriptorSetLayout() const;

		IDObject<VS::AutoCleanupGraphicsPipeline> GetBackgroundLayerGraphicsPipeline();
		IDObject<VS::AutoCleanupGraphicsPipeline> GetUILayerGraphicsPipeline();
		IDObject<VS::AutoCleanupGraphicsPipeline> GetWorldLayerGraphicsPipeline();
		IDObject<VS::AutoCleanupGraphicsPipeline> GetGammaCorrectionGraphicsPipeline();

		IDObject<VS::AutoCleanupPipelineLayout> GetBackgroundLayerGraphicsPipelineLayout();
		IDObject<VS::AutoCleanupPipelineLayout> GetUILayerGraphicsPipelineLayout();
		IDObject<VS::AutoCleanupPipelineLayout> GetWorldLayerGraphicsPipelineLayout();
		IDObject<VS::AutoCleanupPipelineLayout> GetGammaCorrectionGraphicsPipelineLayout();

		const std::vector<std::optional<VS::RenderPassClearValueID>>& GetClearValuesList() const;

	private:
		VS::DeviceMain _device;
		VS::SharedDataMainList _sharedData;

		std::vector<std::optional<VS::RenderPassClearValueID>> _clearValues;
		std::vector<float> _pipelinesAspectRatios;

		size_t _currentPipelineSettings;

		std::vector<IDObject<VS::AutoCleanupRenderPass>> _renderPassList;
		IDObject<VS::AutoCleanupDescriptorSetLayout> _textureDescriptorSetLayout;

		IDObject<VS::AutoCleanupPipelineCache> _backgroundPipelineCache;
		IDObject<VS::AutoCleanupPipelineCache> _uiPipelineCache;
		IDObject<VS::AutoCleanupPipelineCache> _worldLayerPipelineCache;
		IDObject<VS::AutoCleanupPipelineCache> _gammaCorrectionPipelineCache;

		IDObject<VS::AutoCleanupFragmentShaderModule> _standardFragmentShaderID;

		std::optional<uint64_t> _backgroundPipelineCompatibleSavedPos;
		std::optional<uint64_t> _backgroundPipelineCompatibleSavedSize;
		std::optional<uint64_t> _backgroundPipelineCompatibleSavedCRC64WE;

		IDObject<VS::AutoCleanupVertexShaderModule> _backgroundVertexShaderID;
		IDObject<VS::AutoCleanupPipelineLayout> _backgroundPipelineLayout;
		std::vector<IDObject<VS::AutoCleanupGraphicsPipeline>> _backgroundPipelineList;

		std::optional<uint64_t> _uiPipelineCompatibleSavedPos;
		std::optional<uint64_t> _uiPipelineCompatibleSavedSize;
		std::optional<uint64_t> _uiPipelineCompatibleSavedCRC64WE;

		IDObject<VS::AutoCleanupVertexShaderModule> _uiVertexShaderID;
		IDObject<VS::AutoCleanupPipelineLayout> _uiPipelineLayout;
		std::vector<IDObject<VS::AutoCleanupGraphicsPipeline>> _uiPipelineList;

		std::optional<uint64_t> _worldLayerPipelineCompatibleSavedPos;
		std::optional<uint64_t> _worldLayerPipelineCompatibleSavedSize;
		std::optional<uint64_t> _worldLayerPipelineCompatibleSavedCRC64WE;

		IDObject<VS::AutoCleanupVertexShaderModule> _worldLayerVertexShaderID;
		IDObject<VS::PushConstantData> _worldLayerPushConstant;
		IDObject<VS::AutoCleanupPipelineLayout> _worldLayerPipelineLayout;
		std::vector<IDObject<VS::AutoCleanupGraphicsPipeline>> _worldLayerPipelineList;

		std::optional<uint64_t> _gammaCorrectionPipelineCompatibleSavedPos;
		std::optional<uint64_t> _gammaCorrectionPipelineCompatibleSavedSize;
		std::optional<uint64_t> _gammaCorrectionPipelineCompatibleSavedCRC64WE;

		IDObject<VS::AutoCleanupFragmentShaderModule> _gammaCorrectionFragmentShaderID;
		IDObject<VS::AutoCleanupVertexShaderModule> _gammaCorrectionVertexShaderID;
		IDObject<VS::AutoCleanupDescriptorSetLayout> _gammaCorrectionDescriptorSetLayout;
		IDObject<VS::PushConstantData> _gammaCorrectionPushConstant;
		IDObject<VS::AutoCleanupPipelineLayout> _gammaCorrectionPipelineLayout;
		std::vector<IDObject<VS::AutoCleanupGraphicsPipeline>> _gammaCorrectionPipelineList;

		void CreateBackgroundPipelineCacheFile(const std::string& dataFolder);
		void LoadBackgroundPipelineCacheFile(const std::string& dataFolder);
		void SaveBackgroundPipelineCacheFile(const std::string& dataFolder);

		void CreateUILayerPipelineCacheFile(const std::string& dataFolder);
		void LoadUILayerPipelineCacheFile(const std::string& dataFolder);
		void SaveUILayerPipelineCacheFile(const std::string& dataFolder);

		void CreateWorldLayerPipelineCacheFile(const std::string& dataFolder);
		void LoadWorldLayerPipelineCacheFile(const std::string& dataFolder);
		void SaveWorldLayerPipelineCacheFile(const std::string& dataFolder);

		void CreateGammaCorrectionPipelineCacheFile(const std::string& dataFolder);
		void LoadGammaCorrectionPipelineCacheFile(const std::string& dataFolder);
		void SaveGammaCorrectionPipelineCacheFile(const std::string& dataFolder);

		std::vector<char> LoadShaderFile(std::string shaderName);

		IDObject<VS::AutoCleanupRenderPass> CreateRenderPass(const PipelineSettings& pipelineSettings);

		VS::GraphicsPipelineCreationData CompileBackgroundPipelinesCreationData(uint32_t width, uint32_t height, IDObject<VS::AutoCleanupRenderPass> renderPass);
		VS::GraphicsPipelineCreationData CompileUIPipelinesCreationData(uint32_t width, uint32_t height, IDObject<VS::AutoCleanupRenderPass> renderPass);
		VS::GraphicsPipelineCreationData CompileWorldLayerPipelinesCreationData(uint32_t width, uint32_t height, IDObject<VS::AutoCleanupRenderPass> renderPass);
		VS::GraphicsPipelineCreationData CompileGammaCorrectionPipelinesCreationData(uint32_t width, uint32_t height, IDObject<VS::AutoCleanupRenderPass> renderPass);
	};
}