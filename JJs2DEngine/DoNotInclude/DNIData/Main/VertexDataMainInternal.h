#pragma once

#include "../../../Include/Main/VertexDataMainDef.h"

#include "CameraData.h"

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
		IDObject<WorldLayerVertexDataLayerVersionListPointer> AddWorldLayerVersionList(const std::vector<size_t>& versionsMaxObjectAmountsList, size_t addOnReserving);

		void PreRenderingTexturesOwnershipTransfer();

		void TransferVertexData();
		void DrawFrame();
		void IncrementCurrentFrames();

		void TransferPreLoadedTextures();
		void TransferStreamedTextures();

		void SetGammaValue(float newGammaValue);
		float GetGammaValue() const;

		void SetCameraPosition(float X, float Y);
		void SetCameraRotation(float rotation);
		void SetCameraZoom(float zoom);
		void SetCameraAspectRatio(float ratio);

		UiVertexDataLayerVersionListInternal& GetUiVertexDataLayerVersionList(IDObject<UiVertexDataLayerVersionListPointer> ID);
		WorldLayerVertexDataLayerVersionListInternal& GetWorldLayerVertexDataLayerVersionList(IDObject<WorldLayerVertexDataLayerVersionListPointer> ID);

		const UiVertexDataLayerVersionListInternal& GetUiVertexDataLayerVersionList(IDObject<UiVertexDataLayerVersionListPointer> ID) const;
		const WorldLayerVertexDataLayerVersionListInternal& GetWorldLayerVertexDataLayerVersionList(IDObject<WorldLayerVertexDataLayerVersionListPointer> ID) const;

	private:
		TextureDataMainInternal& _textureDataList;
		RenderDataInternal& _renderDataList;
		WindowDataInternal& _windowDataList;

		VS::DataBufferLists _dataBufferList;
		VS::MemoryObjectsList _memoryObjectsList;

		VS::SynchronizationDataLists _synchroList;
		VS::CommandPoolQFGroupList _transferQFGroup;
		VS::CommandPoolQFGroupList _graphicsQFGroup;

		CameraData _camera;

		UnsortedIDVector<UiVertexDataLayerVersionListPointer> _uiLayersList;
		UnsortedIDVector<WorldLayerVertexDataLayerVersionListPointer> _worldLayersList;

		size_t _transferFrameAmount;
		size_t _currentTransferFrame;

		size_t _graphicsFrameAmount;
		size_t _currentGraphicsFrame;

		size_t _transferQueueID;
		size_t _graphicsQueueID;

		float _gammaValue;

		std::vector<VertexLayerOrderID> _layerOrderList;

		std::vector<IDObject<VS::AutoCleanupFence>> _vertexTransferFinishedFences;
		std::vector<IDObject<VS::AutoCleanupSemaphore>> _vertexTransferFinishedSemaphores;
		std::vector<std::optional<size_t>> _lastGraphicsFrameUsingThisTransferFrame;

		std::vector<IDObject<VS::AutoCleanupFence>> _renderingFinishedFences;
		std::vector<IDObject<VS::AutoCleanupFence>> _texturesQueueTrasferFinishedFences;
		std::vector<IDObject<VS::AutoCleanupSemaphore>> _imageAcquiredSemaphores;
		std::vector<IDObject<VS::AutoCleanupSemaphore>> _renderingFinishedSemaphores;
		std::vector<IDObject<VS::AutoCleanupSemaphore>> _transferDataFreeToChangeSemaphores;
		std::vector<IDObject<VS::AutoCleanupSemaphore>> _texturesQueueTrasferFinishedSemaphores;

		IDObject<VS::IRPoolPointer> _transferPoolID;
		std::optional<VS::IRCommandPool> _transferPool;
		std::vector<IDObject<VS::PrimaryIRPointer>> _transferCommandBuffersIDs;

		IDObject<VS::IRPoolPointer> _graphicsPoolID;
		std::optional<VS::IRCommandPool> _graphicsPool;
		std::vector<IDObject<VS::PrimaryIRPointer>> _graphicsCommandBuffersIDs;
	};
}