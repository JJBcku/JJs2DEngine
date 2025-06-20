#pragma once

#include "../VSCommon/VSMemorySizeDef.h"

namespace VulkanSimplified
{
	struct DataBuffersCopyRegionData
	{
		MemorySize srcOffset;
		MemorySize dstOffset;
		MemorySize writeSize;

		DataBuffersCopyRegionData();
	};
}