#pragma once

#include <stdint.h>
#include <vector>
#include <memory>
#include <optional>

#include "../../../Include/Main/UiVertexDataLayerVersionListDef.h"

#include <Miscellaneous/Bool64Def.h>
#include <VulkanSimplified/VSDevice/VSDataBufferLists.h>
#include <VulkanSimplified/VSDevice/VSMemoryObjectsList.h>
#include <VulkanSimplified/VSDevice/VSPrimaryIRCommandBufferDef.h>

#include <CustomLists/IDObject.h>

namespace VulkanSimplified
{
	struct DataBuffersMemoryBarrierData;
}

namespace JJs2DEngine
{
	class UiVertexDataLayerVersionInternal;
	class TextureDataMainInternal;

	class UiVertexDataLayerVersionListInternal
	{
	public:
		UiVertexDataLayerVersionListInternal(TextureDataMainInternal& textureDataList, VS::DataBufferLists& dataBufferList, VS::MemoryObjectsList& memoryObjectsList,
			const std::vector<size_t>& versionsMaxObjectAmountsList, size_t layersDepth, size_t transferFrameAmount);
		~UiVertexDataLayerVersionListInternal();

		UiVertexDataLayerVersionListInternal(const UiVertexDataLayerVersionListInternal& rhs) noexcept = delete;
		UiVertexDataLayerVersionListInternal(UiVertexDataLayerVersionListInternal&& rhs) noexcept = delete;

		UiVertexDataLayerVersionListInternal& operator=(const UiVertexDataLayerVersionListInternal& rhs) noexcept = delete;
		UiVertexDataLayerVersionListInternal& operator=(UiVertexDataLayerVersionListInternal&& rhs) noexcept = delete;

		bool WriteDataToBuffer(size_t transferFrameIndice, VS::PrimaryIRCommandBuffer transferCommandBuffer, bool noChangeOverride);
		VS::DataBuffersMemoryBarrierData GetOwnershipTransferData(size_t transferFrameIndice, size_t transferQueueID, size_t graphicQueueID);
		void RecordDrawCommand(size_t transferFrameIndice, VS::PrimaryIRCommandBuffer graphicsCommandBuffer);

		UiVertexDataLayerVersionInternal& GetLayersVersion(size_t versionIndex);

		const UiVertexDataLayerVersionInternal& GetLayersVersion(size_t versionIndex) const;

		Misc::Bool64 IsOwnedByTransferQueue(size_t transferFrameIndice) const;
		void SetOwnedByTransferQueue(size_t transferFrameIndice, Misc::Bool64Values newValue);

	private:
		VS::DataBufferLists _dataBufferList;
		VS::MemoryObjectsList _memoryObjectsList;

		std::vector<std::unique_ptr<UiVertexDataLayerVersionInternal>> _versionList;
		size_t _activeVersion;

		VS::MemoryAllocationFullID _vertexMemoryID;

		std::optional<std::vector<IDObject<VS::AutoCleanupStagingBuffer>>> _stagingBufferIDs;
		std::optional<VS::MemoryAllocationFullID> _stagingMemoryID;
	};
}
