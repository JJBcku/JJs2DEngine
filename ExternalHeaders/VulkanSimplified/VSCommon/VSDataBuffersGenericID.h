#pragma once

#include <CustomLists/IDObject.h>

#include "../VSDevice/VSDataBufferListsDef.h"

#include <stdint.h>

namespace VulkanSimplified
{
	enum class DataBuffersIDType : uint64_t
	{
		UNKNOWN = 0,
		VERTEX = 1,
		STAGING = 2,
		INDEX = 4,
		UNIFORM = 8,
	};

	union DataBuffersGenericID
	{
		DataBuffersIDType type;

		struct DataBuffersVertexID
		{
			DataBuffersIDType type;
			IDObject<AutoCleanupVertexBuffer> ID;

			DataBuffersVertexID(const IDObject<AutoCleanupVertexBuffer>& ID) noexcept;
			~DataBuffersVertexID() = default;
		} vertexID;

		struct DataBuffersStagingID
		{
			DataBuffersIDType type;
			IDObject<AutoCleanupStagingBuffer> ID;

			DataBuffersStagingID(const IDObject<AutoCleanupStagingBuffer>& ID) noexcept;
			~DataBuffersStagingID() = default;
		} stagingID;

		struct DataBuffersIndexID
		{
			DataBuffersIDType type;
			IDObject<AutoCleanupIndexBuffer> ID;

			DataBuffersIndexID(const IDObject<AutoCleanupIndexBuffer>& ID) noexcept;
			~DataBuffersIndexID() = default;
		} indexID;

		struct DataBuffersUniformID
		{
			DataBuffersIDType type;
			IDObject<AutoCleanupUniformBuffer> ID;

			DataBuffersUniformID(const IDObject<AutoCleanupUniformBuffer>& ID) noexcept;
			~DataBuffersUniformID() = default;
		} uniformID;

		DataBuffersGenericID() noexcept;
		DataBuffersGenericID(const IDObject<AutoCleanupVertexBuffer>& ID) noexcept;
		DataBuffersGenericID(const IDObject<AutoCleanupStagingBuffer>& ID) noexcept;
		DataBuffersGenericID(const IDObject<AutoCleanupIndexBuffer>& ID) noexcept;
		DataBuffersGenericID(const IDObject<AutoCleanupUniformBuffer>& ID) noexcept;

		DataBuffersGenericID(const DataBuffersGenericID& rhs) noexcept;
		DataBuffersGenericID(DataBuffersGenericID&& rhs) noexcept;
		~DataBuffersGenericID();

		DataBuffersGenericID& operator=(const DataBuffersGenericID& rhs) noexcept;
		DataBuffersGenericID& operator=(DataBuffersGenericID&& rhs) noexcept;
	};
}