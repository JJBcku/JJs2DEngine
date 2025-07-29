#pragma once

#include <CustomLists\IDObjectDef.h>

#include "VSSynchronizationDataListsDef.h"

#include <vector>
#include <stdint.h>
#include <limits>

namespace VulkanSimplified
{
	class SynchronizationDataListsInternal;
}

namespace VulkanSimplified
{
	class SynchronizationDataLists
	{
	public:
		SynchronizationDataLists(SynchronizationDataListsInternal& ref);
		~SynchronizationDataLists();

		SynchronizationDataLists& operator=(const SynchronizationDataLists&) noexcept = delete;

		IDObject<AutoCleanupFence> AddFence(bool createSignaled = true, size_t addOnReserving = 0);

		bool WaitOnFences(const std::vector<IDObject<AutoCleanupFence>>& fenceIDs, bool waitForAll,
			uint64_t timeout = std::numeric_limits<uint64_t>::max()) const;
		void ResetFences(const std::vector<IDObject<AutoCleanupFence>>& fenceIDs);

		IDObject<AutoCleanupSemaphore> AddSemaphore(size_t addOnReserving = 0);

		bool RemoveFence(IDObject<AutoCleanupFence> fenceID, bool throwOnIDNotFound);
		bool RemoveSemaphore(IDObject<AutoCleanupSemaphore> semaphoreID, bool throwOnIDNotFound);

	private:
		SynchronizationDataListsInternal& _internal;
	};
}