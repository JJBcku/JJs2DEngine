#pragma once

#include <vector>
#include <optional>
#include <memory>

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
	class BackgroundVertexDataLayerVersionInternal;
	class TextureDataMainInternal;

	struct BackgroundObjectData;

	class BackgroundVertexDataLayerVersionListInternal
	{
	public:
		BackgroundVertexDataLayerVersionListInternal(VS::DataBufferLists dataBufferList, VS::MemoryObjectsList memoryObjectsList, TextureDataMainInternal& textureDataList,
			const std::vector<BackgroundObjectData>& versionDataList, size_t transferFramesAmount);
		~BackgroundVertexDataLayerVersionListInternal();

		bool WriteDataToBuffer(size_t transferFrameIndice, VS::PrimaryIRCommandBuffer transferCommandBuffer, bool noChangeOverride);
		VS::DataBuffersMemoryBarrierData GetOwnershipTransferData(size_t transferFrameIndice, size_t transferQueueID, size_t graphicQueueID);
		void RecordDrawCommand(size_t transferFrameIndice, VS::PrimaryIRCommandBuffer graphicsCommandBuffer);

		void SetActiveVersion(std::optional<size_t> newActiveVersion);
		std::optional<size_t> GetActiveVersion() const;

		Misc::Bool64 IsOwnedByTransferQueue(size_t transferFrameIndice) const;
		void SetOwnedByTransferQueue(size_t transferFrameIndice, Misc::Bool64Values newValue);

	private:
		VS::DataBufferLists _dataBufferList;
		VS::MemoryObjectsList _memoryObjectsList;

		std::vector<std::unique_ptr<BackgroundVertexDataLayerVersionInternal>> _versionList;
		std::optional<size_t> _activeVersion;

		std::vector<std::vector<Misc::Bool64>> _ownedByTransferQueue;
		std::vector<std::vector<Misc::Bool64>> _changed;

		std::vector<std::vector<IDObject<VS::AutoCleanupVertexBuffer>>> _vertexBufferIDs;
		VS::MemoryAllocationFullID _vertexMemoryID;

		std::vector<IDObject<VS::AutoCleanupStagingBuffer>> _stagingBufferIDs;
		std::optional<VS::MemoryAllocationFullID> _stagingMemoryID;
	};
}
