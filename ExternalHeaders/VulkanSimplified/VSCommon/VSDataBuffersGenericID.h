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

		struct
		{
			DataBuffersIDType type;
			IDObject<AutoCleanupVertexBuffer> ID;
		} vertexID;

		struct
		{
			DataBuffersIDType type;
			IDObject<AutoCleanupStagingBuffer> ID;
		} stagingID;

		struct
		{
			DataBuffersIDType type;
			IDObject<AutoCleanupIndexBuffer> ID;
		} indexID;

		struct
		{
			DataBuffersIDType type;
			IDObject<AutoCleanupUniformBuffer> ID;
		} uniformID;

		DataBuffersGenericID() noexcept;
		DataBuffersGenericID(IDObject<AutoCleanupVertexBuffer> ID) noexcept;
		DataBuffersGenericID(IDObject<AutoCleanupStagingBuffer> ID) noexcept;
		DataBuffersGenericID(IDObject<AutoCleanupIndexBuffer> ID) noexcept;
		DataBuffersGenericID(IDObject<AutoCleanupUniformBuffer> ID) noexcept;

		DataBuffersGenericID(const DataBuffersGenericID& rhs) noexcept;
		DataBuffersGenericID(DataBuffersGenericID&& rhs) noexcept;
		~DataBuffersGenericID();

		DataBuffersGenericID& operator=(const DataBuffersGenericID& rhs) noexcept;
		DataBuffersGenericID& operator=(DataBuffersGenericID&& rhs) noexcept;
	};
}