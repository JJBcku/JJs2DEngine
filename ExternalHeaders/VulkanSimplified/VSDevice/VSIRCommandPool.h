#pragma once

#include "VSIRCommandPoolDef.h"

#include "VSWindowListDef.h"

#include <vector>
#include <stdint.h>

namespace VulkanSimplified
{
	class IRCommandPoolInternal;
	class AutoCleanupSemaphore;
}

namespace VulkanSimplified
{
	class PrimaryIRCommandBuffer;
	class SecondaryIRCommandBuffer;

	class IRCommandPool
	{
	public:
		IRCommandPool(IRCommandPoolInternal& ref);
		~IRCommandPool();

		IRCommandPool& operator=(const IRCommandPool&) noexcept = delete;

		std::vector<IDObject<PrimaryIRPointer>> AllocatePrimaryCommandBuffers(uint32_t buffersToAllocate, size_t addOnReserving = 0);
		std::vector<IDObject<SecondaryIRPointer>> AllocateSecondaryCommandBuffers(uint32_t buffersToAllocate, size_t addOnReserving = 0);

		PrimaryIRCommandBuffer GetPrimaryCommandBuffer(IDObject<PrimaryIRPointer> bufferID);
		SecondaryIRCommandBuffer GetSecondaryCommandBuffer(IDObject<SecondaryIRPointer> bufferID);

		const PrimaryIRCommandBuffer GetPrimaryCommandBuffer(IDObject<PrimaryIRPointer> bufferID) const;
		const SecondaryIRCommandBuffer GetSecondaryCommandBuffer(IDObject<SecondaryIRPointer> bufferID) const;

		void ResetCommandPool(bool freeResources);

		bool PresentSwapchainToQueue(IDObject<WindowPointer> windowID, const std::vector<IDObject<AutoCleanupSemaphore>>& waitSemaphoreIDs,
			uint32_t imageIndex);

		void RecordExecuteSecondaryBufferCommand(IDObject<PrimaryIRPointer> primaryBufferID, const std::vector<IDObject<SecondaryIRPointer>>& secondaryBufferIDs);

	private:
		IRCommandPoolInternal& _internal;
	};
}