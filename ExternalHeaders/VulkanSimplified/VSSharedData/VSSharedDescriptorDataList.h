#pragma once

#include <CustomLists\IDObjectDef.h>

#include "VSSharedDescriptorDataListDef.h"

namespace VulkanSimplified
{
	class SharedDescriptorDataListInternal;
}

namespace VulkanSimplified
{
	class SharedDescriptorDataList
	{
	public:
		SharedDescriptorDataList(SharedDescriptorDataListInternal& ref);
		~SharedDescriptorDataList();

		IDObject<DescriptorSetLayoutBindingData> AddDescriptorSetLayoutBindingsData(DescriptorTypeFlagBits descriptorType,
			uint32_t descriptorAmount, ShaderTypeFlags shaderStageFlags, size_t addOnReserving = 0);

	private:
		SharedDescriptorDataListInternal& _internal;
	};
}