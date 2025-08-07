#pragma once

namespace VulkanSimplified
{
	struct ImageDataListsInitialCapacities
	{
		size_t colorRenderTargetsListInitialCapacity;
		size_t depthStencilRenderTargetsListInitialCapacity;
		size_t resolveRenderTargetsListInitialCapacity;

		size_t twoDTexturesListInitialCapacity;
		size_t twoDArrayTexturesListInitialCapacity;

		size_t framebufferListInitialCapacity;
		size_t samplerListInitialCapacity;

		ImageDataListsInitialCapacities();
	};
}