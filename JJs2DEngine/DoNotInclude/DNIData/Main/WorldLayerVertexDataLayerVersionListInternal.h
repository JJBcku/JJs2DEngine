#pragma once

#include <Miscellaneous/Bool64Def.h>
#include <VulkanSimplified/VSDevice/VSDataBufferLists.h>
#include <VulkanSimplified/VSDevice/VSMemoryObjectsList.h>
#include <VulkanSimplified/VSDevice/VSPrimaryIRCommandBufferDef.h>

#include <CustomLists/IDObject.h>

#include <stdint.h>
#include <vector>
#include <memory>
#include <optional>

namespace VulkanSimplified
{
	struct DataBuffersMemoryBarrierData;
}

namespace JJs2DEngine
{
	class WorldLayerVertexDataLayerVersionInternal;
	class TextureDataMainInternal;

	class WorldLayerVertexDataLayerVersionListInternal
	{
	public:
		WorldLayerVertexDataLayerVersionListInternal(TextureDataMainInternal& textureDataList, VS::DataBufferLists& dataBufferList, VS::MemoryObjectsList& memoryObjectsList,
			const std::vector<size_t>& versionsMaxObjectAmountsList, size_t layersDepth, size_t transferFrameAmount);
		~WorldLayerVertexDataLayerVersionListInternal();

		WorldLayerVertexDataLayerVersionListInternal(const WorldLayerVertexDataLayerVersionListInternal& rhs) noexcept = delete;
		WorldLayerVertexDataLayerVersionListInternal(WorldLayerVertexDataLayerVersionListInternal&& rhs) noexcept = delete;

		WorldLayerVertexDataLayerVersionListInternal& operator=(const WorldLayerVertexDataLayerVersionListInternal& rhs) noexcept = delete;
		WorldLayerVertexDataLayerVersionListInternal& operator=(WorldLayerVertexDataLayerVersionListInternal&& rhs) noexcept = delete;

		bool WriteDataToBuffer(size_t transferFrameIndice, VS::PrimaryIRCommandBuffer transferCommandBuffer);
		VS::DataBuffersMemoryBarrierData GetOwnershipTransferData(size_t transferFrameIndice, size_t transferQueueID, size_t graphicQueueID);
		void RecordDrawCommand(size_t transferFrameIndice, VS::PrimaryIRCommandBuffer graphicsCommandBuffer);

		WorldLayerVertexDataLayerVersionInternal& GetLayersVersion(size_t versionIndex);

		const WorldLayerVertexDataLayerVersionInternal& GetLayersVersion(size_t versionIndex) const;

		Misc::Bool64 IsOwnedByTransferQueue(size_t transferFrameIndice) const;
		void SetOwnedByTransferQueue(size_t transferFrameIndice, Misc::Bool64Values newValue);

	private:
		VS::DataBufferLists _dataBufferList;
		VS::MemoryObjectsList _memoryObjectsList;

		std::vector<std::unique_ptr<WorldLayerVertexDataLayerVersionInternal>> _versionList;
		size_t _activeVersion;

		VS::MemoryAllocationFullID _vertexMemoryID;

		std::vector<IDObject<VS::AutoCleanupStagingBuffer>> _stagingBufferIDs;
		std::optional<VS::MemoryAllocationFullID> _stagingMemoryID;
	};
}
