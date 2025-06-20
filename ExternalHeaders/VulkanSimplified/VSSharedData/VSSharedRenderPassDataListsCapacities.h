#pragma once

namespace VulkanSimplified
{
	struct SharedRenderPassDataListsCapacities
	{
		size_t sharedRenderPassAttachmentsInitialCapacity;
		size_t sharedRenderPassReferencesInitialCapacity;
		size_t sharedSubpassDependenciesInitialCapacity;

		size_t doubleColorClearValueInitialCapacity;
		size_t int64ColorClearValueInitialCapacity;
		size_t uint64ColorClearValueInitialCapacity;

		size_t floatColorClearValueInitialCapacity;
		size_t intColorClearValueInitialCapacity;
		size_t uintColorClearValueInitialCapacity;

		size_t depthStencilColorClearValueInitialCapacity;

		SharedRenderPassDataListsCapacities();
	};
}