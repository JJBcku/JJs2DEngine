#pragma once

#include <vector>

#include "VSDataBufferListsDef.h"

#include "../VSCommon/VSMemorySizeDef.h"
#include "../VSCommon/VSMemoryAllocationFullID.h"

#include "VSIndexTypeDef.h"

namespace VulkanSimplified
{
	class DataBufferListsInternal;
}

namespace VulkanSimplified
{
	class DataBufferLists
	{
	public:
		DataBufferLists(DataBufferListsInternal& ref);
		~DataBufferLists();

		DataBufferLists& operator=(const DataBufferLists& rhs) noexcept = delete;

		IDObject<AutoCleanupVertexBuffer> AddVertexBuffer(MemorySize size, const std::vector<size_t>& queuesUsingBuffer, size_t addOnReserving = 0);
		IDObject<AutoCleanupStagingBuffer> AddStagingBuffer(MemorySize size, const std::vector<size_t>& queuesUsingBuffer, size_t addOnReserving = 0);
		IDObject<AutoCleanupIndexBuffer> AddIndexBuffer(size_t indicesCount, IndexType indexType,
			const std::vector<size_t>& queuesUsingBuffer, size_t addOnReserving = 0);
		IDObject<AutoCleanupUniformBuffer> AddUniformBuffer(MemorySize size, const std::vector<size_t>& queuesUsingBuffer, size_t addOnReserving = 0);

		void BindVertexBuffer(IDObject<AutoCleanupVertexBuffer> bufferID, MemoryAllocationFullID allocationID, size_t addOnReserving = 0);
		void BindStagingBuffer(IDObject<AutoCleanupStagingBuffer> bufferID, MemoryAllocationFullID allocationID, size_t addOnReserving = 0);
		void BindIndexBuffer(IDObject<AutoCleanupIndexBuffer> bufferID, MemoryAllocationFullID allocationID, size_t addOnReserving = 0);
		void BindUniformBuffer(IDObject<AutoCleanupUniformBuffer> bufferID, MemoryAllocationFullID allocationID, size_t addOnReserving = 0);

		bool RemoveVertexBuffer(IDObject<AutoCleanupVertexBuffer> bufferID, bool throwOnIDNotFound);
		bool RemoveStagingBuffer(IDObject<AutoCleanupStagingBuffer> bufferID, bool throwOnIDNotFound);
		bool RemoveIndexBuffer(IDObject<AutoCleanupIndexBuffer> bufferID, bool throwOnIDNotFound);
		bool RemoveUniformBuffer(IDObject<AutoCleanupUniformBuffer> bufferID, bool throwOnIDNotFound);

		uint32_t GetVertexBuffersMemoryTypeMask(IDObject<AutoCleanupVertexBuffer> bufferID) const;
		MemorySize GetVertexBuffersSize(IDObject<AutoCleanupVertexBuffer> bufferID) const;
		MemorySize GetVertexBuffersRequiredAligment(IDObject<AutoCleanupVertexBuffer> bufferID) const;

		uint32_t GetStagingBuffersMemoryTypeMask(IDObject<AutoCleanupStagingBuffer> bufferID) const;
		MemorySize GetStagingBuffersSize(IDObject<AutoCleanupStagingBuffer> bufferID) const;
		MemorySize GetStagingBuffersRequiredAligment(IDObject<AutoCleanupStagingBuffer> bufferID) const;

		uint32_t GetIndexBuffersMemoryTypeMask(IDObject<AutoCleanupIndexBuffer> bufferID) const;
		MemorySize GetIndexBuffersSize(IDObject<AutoCleanupIndexBuffer> bufferID) const;
		MemorySize GetIndexBuffersRequiredAligment(IDObject<AutoCleanupIndexBuffer> bufferID) const;

		uint32_t GetUniformBuffersMemoryTypeMask(IDObject<AutoCleanupUniformBuffer> bufferID) const;
		MemorySize GetUniformBuffersSize(IDObject<AutoCleanupUniformBuffer> bufferID) const;
		MemorySize GetUniformBuffersRequiredAligment(IDObject<AutoCleanupUniformBuffer> bufferID) const;

		void WriteToVertexBuffer(IDObject<AutoCleanupVertexBuffer> bufferID, MemorySize writeOffset, const unsigned char& writeData,
			MemorySize dataSize);
		void WriteToStagingBuffer(IDObject<AutoCleanupStagingBuffer> bufferID, MemorySize writeOffset, const unsigned char& writeData,
			MemorySize dataSize);
		void WriteToIndexBuffer(IDObject<AutoCleanupIndexBuffer> bufferID, MemorySize writeOffset, const unsigned char& writeData,
			MemorySize dataSize);
		void WriteToUniformBuffer(IDObject<AutoCleanupUniformBuffer> bufferID, MemorySize writeOffset, const unsigned char& writeData,
			MemorySize dataSize);

	private:
		DataBufferListsInternal& _internal;
	};
}