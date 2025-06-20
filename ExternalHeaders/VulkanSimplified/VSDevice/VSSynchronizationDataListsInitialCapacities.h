#pragma once

namespace VulkanSimplified
{
	struct SynchronizationDataListsInitialCapacities
	{
		size_t fenceListInitialCapacity;
		size_t semaphoreListInitialCapacity;

		SynchronizationDataListsInitialCapacities();
		~SynchronizationDataListsInitialCapacities();
	};
}