#pragma once

namespace VulkanSimplified
{
	struct QueueOwnershipTransferData
	{
		size_t srcQueueIndex;
		size_t dstQueueIndex;

		QueueOwnershipTransferData();
		QueueOwnershipTransferData(size_t srcQueue, size_t dstQueue);
	};
}