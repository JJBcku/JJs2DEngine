#pragma once

#include <CustomLists/IDObjectDef.h>

#include "VSWindowListDef.h"
#include "VSNIRCommandPoolDef.h"

#include <vector>
#include <stdint.h>

namespace VulkanSimplified
{
	class NIRCommandPoolInternal;
	class AutoCleanupSemaphore;
}

namespace VulkanSimplified
{
	class PrimaryNIRCommandBuffer;
	class SecondaryNIRCommandBuffer;

	class NIRCommandPool
	{
	public:
		NIRCommandPool(NIRCommandPoolInternal& ref);
		~NIRCommandPool();

		NIRCommandPool(const NIRCommandPool&) noexcept = default;
		NIRCommandPool(NIRCommandPool&&) noexcept = default;

		NIRCommandPool& operator=(const NIRCommandPool&) noexcept = delete;
		NIRCommandPool& operator=(NIRCommandPool&&) noexcept = delete;

		std::vector<IDObject<PrimaryNIRPointer>> AllocatePrimaryCommandBuffers(uint32_t buffersToAllocate, size_t addOnReserving = 0);
		std::vector<IDObject<SecondaryNIRPointer>> AllocateSecondaryCommandBuffers(uint32_t buffersToAllocate, size_t addOnReserving = 0);

		PrimaryNIRCommandBuffer GetPrimaryCommandBuffer(IDObject<PrimaryNIRPointer> bufferID);
		SecondaryNIRCommandBuffer GetSecondaryCommandBuffer(IDObject<SecondaryNIRPointer> bufferID);

		const PrimaryNIRCommandBuffer GetPrimaryCommandBuffer(IDObject<PrimaryNIRPointer> bufferID) const;
		const SecondaryNIRCommandBuffer GetSecondaryCommandBuffer(IDObject<SecondaryNIRPointer> bufferID) const;

		void ResetCommandPool(bool freeResources);

		bool PresentSwapchainToQueue(IDObject<WindowPointer> windowID, const std::vector<IDObject<AutoCleanupSemaphore>>& waitSemaphoreIDs,
			uint32_t imageIndex);

		void RecordExecuteSecondaryBufferCommand(IDObject<PrimaryNIRPointer> primaryBufferID, const std::vector<IDObject<SecondaryNIRPointer>>& secondaryBufferIDs);

	private:
		NIRCommandPoolInternal& _internal;
	};
}