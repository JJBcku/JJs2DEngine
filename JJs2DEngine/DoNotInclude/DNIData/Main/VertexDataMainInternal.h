#pragma once

#include "../../../Include/Main/VertexDataMainDef.h"

#include <CustomLists/UnsortedIDVector.h>
#include <VulkanSimplified/VSDevice/VSDataBufferLists.h>
#include <VulkanSimplified/VSDevice/VSMemoryObjectsList.h>
#include <VulkanSimplified/VSDevice/VSSynchronizationDataLists.h>
#include <VulkanSimplified/VSDevice/VSCommandPoolQFGroupList.h>
#include <VulkanSimplified/VSDevice/VSIRCommandPool.h>
#include <VulkanSimplified/VSDevice/VSPrimaryIRCommandBuffer.h>

#include <stdint.h>
#include <vector>
#include <optional>

namespace JJs2DEngine
{
	union VertexLayerOrderID;

	class TextureDataMainInternal;

	class VertexDataMainInternal
	{
	public:
		VertexDataMainInternal(TextureDataMainInternal& textureDataList, VS::DataBufferLists dataBufferList, VS::MemoryObjectsList memoryObjectsList, VS::SynchronizationDataLists synchroList,
			VS::CommandPoolQFGroupList transferQFGroup, uint32_t transferFrameAmount, size_t transferQueueID);
		~VertexDataMainInternal();

		IDObject<UiVertexDataLayerVersionListPointer> AddUiLayerVersionList(const std::vector<size_t>& versionsMaxVerticesList, size_t addOnReserving);

		void TransferVertexData();

		UiVertexDataLayerVersionListInternal& GetUiVertexDataLayerVersionList(IDObject<UiVertexDataLayerVersionListPointer> ID);

		const UiVertexDataLayerVersionListInternal& GetUiVertexDataLayerVersionList(IDObject<UiVertexDataLayerVersionListPointer> ID) const;

	private:
		TextureDataMainInternal& _textureDataList;
		VS::DataBufferLists _dataBufferList;
		VS::MemoryObjectsList _memoryObjectsList;
		VS::SynchronizationDataLists _synchroList;
		VS::CommandPoolQFGroupList _transferQFGroup;

		UnsortedIDVector<UiVertexDataLayerVersionListPointer> _uiLayersList;
		size_t _transferFrameAmount;
		size_t _currentTranferFrame;

		size_t _transferQueueID;

		std::vector<VertexLayerOrderID> _layerOrderList;
		std::vector<IDObject<VS::AutoCleanupFence>> _vertexTransferFinishedFences;
		std::vector<IDObject<VS::AutoCleanupSemaphore>> _vertexTransferFinishedSemaphores;

		IDObject<VS::IRPoolPointer> _transferPoolID;
		std::optional<VS::IRCommandPool> _transferPool;
		std::vector<IDObject<VS::PrimaryIRPointer>> _transferCommandBuffersIDs;
	};
}