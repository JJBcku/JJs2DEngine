#pragma once

#include "VSQueueOwnershipTransferData.h"

#include "../VSCommon/VSAccessFlagsDef.h"
#include "../VSCommon/VSImageLayoutFlagsDef.h"
#include "../VSCommon/VSImagesGenericID.h"

#include <optional>

namespace VulkanSimplified
{
	struct ImagesMemoryBarrierData
	{
		AccessFlags srcAccess;
		AccessFlags dstAccess;
		ImageLayoutFlags oldLayout;
		ImageLayoutFlags newLayout;
		std::optional<QueueOwnershipTransferData> queueData;
		ImagesGenericID imageID;

		ImagesMemoryBarrierData();
	};
}