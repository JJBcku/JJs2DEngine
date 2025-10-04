#pragma once

#include "../../../Include/Main/VertexDataMainDef.h"

#include <CustomLists/UnsortedIDVector.h>
#include <VulkanSimplified/VSDevice/VSDataBufferLists.h>
#include <VulkanSimplified/VSDevice/VSMemoryObjectsList.h>
#include <VulkanSimplified/VSDevice/VSSynchronizationDataLists.h>

#include <stdint.h>
#include <vector>

namespace JJs2DEngine
{
	union VertexLayerOrderID;

	class TextureDataMainInternal;

	class VertexDataMainInternal
	{
	public:
		VertexDataMainInternal(TextureDataMainInternal& textureDataList, VS::DataBufferLists dataBufferList, VS::MemoryObjectsList memoryObjectsList, VS::SynchronizationDataLists synchroList,
			size_t transferFrameAmount);
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

		UnsortedIDVector<UiVertexDataLayerVersionListPointer> _uiLayersList;
		size_t _transferFrameAmount;
		size_t _currentTranferFrame;

		std::vector<VertexLayerOrderID> _layerOrderList;
		std::vector<IDObject<VS::AutoCleanupFence>> _vertexTransferFinishedFences;
		std::vector<IDObject<VS::AutoCleanupSemaphore>> _vertexTransferFinshedSemaphores;
	};
}