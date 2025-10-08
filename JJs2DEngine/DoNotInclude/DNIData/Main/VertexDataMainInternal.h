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
	class RenderDataInternal;
	class WindowDataInternal;

	class VertexDataMainInternal
	{
	public:
		VertexDataMainInternal(TextureDataMainInternal& textureDataList, RenderDataInternal& renderDataList, WindowDataInternal& windowDataList,
			VS::DataBufferLists dataBufferList, VS::MemoryObjectsList memoryObjectsList, VS::SynchronizationDataLists synchroList,
			VS::CommandPoolQFGroupList transferQFGroup, uint32_t transferFrameAmount, size_t transferQueueID,
			VS::CommandPoolQFGroupList graphicQFGroup, uint32_t graphicsFrameAmount, size_t graphicQueueID);
		~VertexDataMainInternal();

		IDObject<UiVertexDataLayerVersionListPointer> AddUiLayerVersionList(const std::vector<size_t>& versionsMaxObjectAmountsList, size_t addOnReserving);

		void PreRenderingTextureOwnershipTransfer();

		void TransferVertexData();
		void DrawFrame();
		void IncrementCurrentFrames();

		UiVertexDataLayerVersionListInternal& GetUiVertexDataLayerVersionList(IDObject<UiVertexDataLayerVersionListPointer> ID);

		const UiVertexDataLayerVersionListInternal& GetUiVertexDataLayerVersionList(IDObject<UiVertexDataLayerVersionListPointer> ID) const;

	private:
		TextureDataMainInternal& _textureDataList;
		RenderDataInternal& _renderDataList;
		WindowDataInternal& _windowDataList;

		VS::DataBufferLists _dataBufferList;
		VS::MemoryObjectsList _memoryObjectsList;

		VS::SynchronizationDataLists _synchroList;
		VS::CommandPoolQFGroupList _transferQFGroup;
		VS::CommandPoolQFGroupList _graphicsQFGroup;

		UnsortedIDVector<UiVertexDataLayerVersionListPointer> _uiLayersList;

		size_t _transferFrameAmount;
		size_t _currentTransferFrame;

		size_t _graphicsFrameAmount;
		size_t _currentGraphicsFrame;

		size_t _transferQueueID;
		size_t _graphicsQueueID;

		std::vector<VertexLayerOrderID> _layerOrderList;

		std::vector<IDObject<VS::AutoCleanupFence>> _vertexTransferFinishedFences;
		std::vector<IDObject<VS::AutoCleanupSemaphore>> _vertexTransferFinishedSemaphores;

		std::vector<IDObject<VS::AutoCleanupFence>> _renderingFinishedFences;
		std::vector<IDObject<VS::AutoCleanupSemaphore>> _imageAcquiredSemaphores;
		std::vector<IDObject<VS::AutoCleanupSemaphore>> _renderingFinishedSemaphores;
		std::vector<IDObject<VS::AutoCleanupSemaphore>> _transferDataFreeToChangeSemaphores;

		IDObject<VS::IRPoolPointer> _transferPoolID;
		std::optional<VS::IRCommandPool> _transferPool;
		std::vector<IDObject<VS::PrimaryIRPointer>> _transferCommandBuffersIDs;

		IDObject<VS::IRPoolPointer> _graphicsPoolID;
		std::optional<VS::IRCommandPool> _graphicsPool;
		std::vector<IDObject<VS::PrimaryIRPointer>> _graphicsCommandBuffersIDs;
	};
}