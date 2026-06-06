#pragma once

#include "VSQueueOwnershipTransferData.h"

#include "../VSCommon/VSAccessFlagsDef.h"
#include "../VSCommon/VSDataBuffersGenericID.h"

#include <optional>

namespace VulkanSimplified
{
	struct DataBuffersMemoryBarrierData
	{
		AccessFlags srcAccess;
		AccessFlags dstAccess;
		std::optional<QueueOwnershipTransferData> queueData;
		DataBuffersGenericID bufferID;

		DataBuffersMemoryBarrierData();
		~DataBuffersMemoryBarrierData() = default;

		DataBuffersMemoryBarrierData(const DataBuffersMemoryBarrierData&) noexcept = default;
		DataBuffersMemoryBarrierData(DataBuffersMemoryBarrierData&&) noexcept = default;

		DataBuffersMemoryBarrierData& operator=(const DataBuffersMemoryBarrierData&) noexcept = default;
		DataBuffersMemoryBarrierData& operator=(DataBuffersMemoryBarrierData&&) noexcept = default;
	};
}