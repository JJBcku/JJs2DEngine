#pragma once

namespace VulkanSimplified
{
	struct SharedPipelineDataListsssCapacities
	{
		size_t initialSpecializationElementsCapacity;
		size_t initialShaderPipelineInfoCapacity;

		size_t initialVertexAttributeInfoCapacity;
		size_t initialVertexBindingInfoCapacity;
		size_t initialVertexInputPipelineInfoCapacity;

		size_t initialPipelineScissorDataCapacity;
		size_t initialPipelineViewportDataCapacity;

		size_t initialPipelineInputAssemblyInfoCapacity;
		size_t initialPipelineRasterizationInfoCapacity;
		size_t initialPipelineMultisampleInfoCapacity;
		size_t initialPipelineDepthStencilInfoCapacity;
		size_t initialPipelineColorBlendAttachmentInfoCapacity;

		size_t initialPushConstantDataCapacity;

		SharedPipelineDataListsssCapacities();
	};
}