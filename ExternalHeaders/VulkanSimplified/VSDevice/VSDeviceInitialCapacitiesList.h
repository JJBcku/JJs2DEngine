#pragma once

#include "VSWindowListInitialCapacities.h"
#include "VSShaderListsInitialCapacitiesList.h"

#include "VSDescriptorListsInitialCapacities.h"
#include "VSRenderPassListInitialCapacities.h"
#include "VSPipelineDataListsInitialCapacities.h"

#include "VSMemoryObjectsListInitialCapacities.h"
#include "VSDataBufferListsInitialCapacities.h"
#include "VSImageDataListsInitialCapacities.h"

#include "VSSynchronizationDataListsInitialCapacities.h"
#include "VSCommandPoolMainListInitialCapacity.h"

namespace VulkanSimplified
{
	struct DeviceInitialCapacitiesList
	{
		WindowListInitialCapacities windowList;
		ShaderListsInitialCapacitiesList shaderLists;

		DescriptorListsInitialCapacities descriptorLists;
		RenderPassDataListsInitialCapacities renderPassLists;
		PipelineDataListsInitialCapacities pipelineDataLists;

		MemoryObjectsListInitialCapacities memoryObjectsList;
		DataBufferListsInitialCapacities dataBufferLists;
		ImageDataListsInitialCapacities imageLists;

		SynchronizationDataListsInitialCapacities synchronizationLists;
		CommandPoolMainListInitialCapacity commandPoolMainList;

		DeviceInitialCapacitiesList() = default;
	};
}
