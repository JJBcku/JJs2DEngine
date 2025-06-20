#pragma once

#include <CustomLists/IDObjectDef.h>

#include "VSCommandBufferBase.h"
#include "VSWindowListDef.h"
#include "VSRenderPassDataListDef.h"
#include "VSImageDataListsDef.h"

#include "../VSSharedData/VSRenderPassClearValueIDDef.h"

namespace VulkanSimplified
{
	class PrimaryNIRCommandBufferInternal;
}

namespace VulkanSimplified
{
	struct QueueOwnershipTransferData;

	class PrimaryNIRCommandBuffer : public CommandBufferBase
	{
	public:
		PrimaryNIRCommandBuffer(PrimaryNIRCommandBufferInternal& ref);
		~PrimaryNIRCommandBuffer();

		PrimaryNIRCommandBuffer& operator=(const PrimaryNIRCommandBuffer&) noexcept = delete;

		void BeginRenderPass(IDObject<AutoCleanupRenderPass> renderPassID, IDObject<AutoCleanupFramebuffer> framebufferID,
			uint32_t startX, uint32_t startY, uint32_t width, uint32_t height, const std::vector<std::optional<RenderPassClearValueID>>& clearValues, bool usesSecondaryBuffers = false);
		void EndRenderPass();

		void TransitionSwapchainImageToTrasferDestination(IDObject<WindowPointer> windowID, std::optional<QueueOwnershipTransferData> queueOwnershipTransferData, uint32_t imagesIndex);
		void TransitionSwapchainImageToPresent(IDObject<WindowPointer> windowID, std::optional<QueueOwnershipTransferData> queueOwnershipTransferData, uint32_t imagesIndex);

		void BlitColorRenderTargetToSwapchainImage(IDObject<WindowPointer> windowID, IDObject<AutoCleanupColorRenderTargetImage> imageID,
			uint32_t startX, uint32_t startY, uint32_t width, uint32_t height, uint32_t swapchainImageIndex);
		void BlitResolveRenderTargetToSwapchainImage(IDObject<WindowPointer> windowID,
			IDObject<AutoCleanupResolveRenderTargetImage> imageID, uint32_t startX, uint32_t startY, uint32_t width, uint32_t height,
			uint32_t swapchainImageIndex);

	private:
		PrimaryNIRCommandBufferInternal& _internal;
	};
}