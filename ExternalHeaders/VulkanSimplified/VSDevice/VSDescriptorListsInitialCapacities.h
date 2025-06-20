#pragma once

namespace VulkanSimplified
{
	struct DescriptorListsInitialCapacities
	{
		size_t descriptorSetListInitialCapacity;

		size_t noIndividualFreeingDescriptorPoolsListInitialCapacity;
		size_t individualFreeingDescriptorPoolsListInitialCapacity;

		DescriptorListsInitialCapacities();
	};
}