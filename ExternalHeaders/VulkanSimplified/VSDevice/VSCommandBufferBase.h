#pragma once

#include <CustomLists/IDObjectDef.h>

#include "VSPipelineDataListsDef.h"
#include "VSRenderPassDataListDef.h"
#include "VSSynchronizationDataListsDef.h"
#include "VSWindowListDef.h"
#include "VSDataBufferListsDef.h"
#include "VSImageDataListsDef.h"
#include "VSIndexTypeDef.h"
#include "VSDescriptorDataListsDef.h"

#include "VSCommandBufferUsage.h"

#include "../VSCommon/VSPipelineStageFlagsDef.h"
#include "../VSCommon/VSMemorySizeDef.h"

#include <optional>
#include <vector>
#include <utility>
#include <stdint.h>

namespace VulkanSimplified
{
	class CommandBufferBaseInternal;
}

namespace VulkanSimplified
{
	struct DataBuffersCopyRegionData;

	struct GlobalMemoryBarrierData;
	struct DataBuffersMemoryBarrierData;
	struct ImagesMemoryBarrierData;

	union DescriptorPoolGenericID;

	class CommandBufferBase
	{
	public:
		CommandBufferBase(CommandBufferBaseInternal& ref);
		~CommandBufferBase();

		CommandBufferBase& operator=(const CommandBufferBase& rhs) noexcept = delete;

		void EndRecording();

		void BindGraphicsPipeline(IDObject<AutoCleanupGraphicsPipeline> pipelineID);

		void Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance);
		void DrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t firstVertexOffset, uint32_t firstInstance);

		bool AcquireNextImage(uint64_t timeout, std::optional<IDObject<AutoCleanupSemaphore>> semaphoreID,
			std::optional<IDObject<AutoCleanupFence>> fenceID, uint32_t& returnIndex, IDObject<WindowPointer> windowID);

		void TransferDataToVertexBuffer(IDObject<AutoCleanupStagingBuffer> srcBufferID, IDObject<AutoCleanupVertexBuffer> dstBufferID,
			const DataBuffersCopyRegionData& copyRegion);
		void TransferDataListToVertexBuffer(IDObject<AutoCleanupStagingBuffer> srcBufferID,
			IDObject<AutoCleanupVertexBuffer> dstBufferID, const std::vector<DataBuffersCopyRegionData>& copyRegionsList);

		void TransferDataToIndexBuffer(IDObject<AutoCleanupStagingBuffer> srcBufferID, IDObject<AutoCleanupIndexBuffer> dstBufferID,
			const DataBuffersCopyRegionData& copyRegion);
		void TransferDataListToIndexBuffer(IDObject<AutoCleanupStagingBuffer> srcBufferID,
			IDObject<AutoCleanupIndexBuffer> dstBufferID, const std::vector<DataBuffersCopyRegionData>& copyRegionsList);

		void TransferDataToUniformBuffer(IDObject<AutoCleanupStagingBuffer> srcBufferID,
			IDObject<AutoCleanupUniformBuffer> dstBufferID, const DataBuffersCopyRegionData& copyRegion);
		void TransferDataListToUniformBuffer(IDObject<AutoCleanupStagingBuffer> srcBufferID,
			IDObject<AutoCleanupUniformBuffer> dstBufferID, const std::vector<DataBuffersCopyRegionData>& copyRegionsList);

		void TransferDataTo2dTextureSimple(IDObject<AutoCleanupStagingBuffer> srcBufferID,
			IDObject<AutoCleanup2DTexture> dstTextureID, uint32_t mipLevel);

		void TransferDataTo2dArrayTextureSingleLayer(IDObject<AutoCleanupStagingBuffer> srcBufferID, size_t bufferOffset, size_t dataSize,
			IDObject<AutoCleanup2DArrayTexture> dstTextureID, uint32_t startWidth, uint32_t startHeight, uint32_t width, uint32_t height, uint32_t mipLevel, uint32_t layer);

		void BlitDataBetween2DTexturesSimple(IDObject<AutoCleanup2DTexture> srcTexureID, uint32_t srcMipLevel,
			IDObject<AutoCleanup2DTexture> dstTextureID, uint32_t dstMipLevel);

		void CreatePipelineBarrier(PipelineStageFlags srcStages, PipelineStageFlags dstStages, const std::vector<GlobalMemoryBarrierData>& globalMemoryBarrierData,
			const std::vector<DataBuffersMemoryBarrierData>& dataBuffersBarrierData, const std::vector<ImagesMemoryBarrierData>& imageBarrierData);

		void BindVertexBuffers(uint32_t firstBinding,
			const std::vector<std::pair<IDObject<AutoCleanupVertexBuffer>, MemorySize>>& buffersDataList);
		void BindIndexBuffer(IDObject<AutoCleanupIndexBuffer> bufferID, MemorySize buffersOffset, IndexType indexType);

		void BindDescriptorSetsToGraphicsPipeline(IDObject<AutoCleanupPipelineLayout> pipelineLayoutID, uint32_t firstSet,
			DescriptorPoolGenericID descriptorPoolID, const std::vector<IDObject<AutoCleanupDescriptorSet>>& descriptorSetIDList,
			const std::vector<uint32_t>& dynamicOffsetList);

	private:
		CommandBufferBaseInternal& _internal;
	};
}