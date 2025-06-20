#pragma once

#include <CustomLists/IDObject.h>

#include "VSArbitraryShaderID.h"

#include <stdint.h>
#include <vector>
#include <optional>

namespace VulkanSimplified
{
	struct ShaderStageData;

	struct PipelineViewportData;
	struct PipelineScissorData;

	struct ShaderSpecializationElement;
	struct SharedShaderPipelineData;

	struct VertexInputSharedPipelineData;

	struct PipelineInputAssemblyData;
	struct PipelineRasterizationData;
	struct PipelineMultisampleData;
	struct PipelineDepthStencilStateData;
	struct PipelineColorBlendAttachment;

	class AutoCleanupPipelineLayout;
	class AutoCleanupGraphicsPipeline;
	class AutoCleanupRenderPass;
}

namespace VulkanSimplified
{
	struct ShaderSpecializationData
	{
		std::vector<IDObject<ShaderSpecializationElement>> specializationElements;
		size_t dataSize;
		const void* data;

		ShaderSpecializationData();
	};

	struct ShaderStageData
	{
		IDObject<SharedShaderPipelineData> sharedData;
		ArbitraryShaderID shaderDeviceID;

		std::optional<ShaderSpecializationData> specializationData;

		ShaderStageData() = default;
	};

	struct ViewportStatePipelineDataPoint
	{
		IDObject<PipelineViewportData> viewport;
		IDObject<PipelineScissorData> scissor;

		ViewportStatePipelineDataPoint() = default;
	};

	enum class PipelineDerrivationSettings : uint64_t
	{
		DO_NOT_DERRIVE = 0,
		PIPELINE_ID = 1,
		PIPELINE_INDEX = 2,
	};

	union GraphicsPipelineDerrivationData
	{
		PipelineDerrivationSettings settings;

		struct
		{
			PipelineDerrivationSettings settings;
			IDObject<AutoCleanupGraphicsPipeline> ID;
		} pipelineID;

		struct
		{
			PipelineDerrivationSettings settings;
			size_t index;
		} pipelineIndex;

		GraphicsPipelineDerrivationData();
	};

	struct GraphicsPipelineCreationData
	{
		std::vector<ShaderStageData> shaderStages;
		IDObject<VertexInputSharedPipelineData> vertexInputData;
		IDObject<PipelineInputAssemblyData> inputAssemblyData;
		std::vector<ViewportStatePipelineDataPoint> viewportData;
		IDObject<PipelineRasterizationData> rasterizationData;
		IDObject<PipelineMultisampleData> samplingData;
		std::optional<IDObject<PipelineDepthStencilStateData>> depthStencilData;
		std::vector<IDObject<PipelineColorBlendAttachment>> colorBlendingData;

		IDObject<AutoCleanupPipelineLayout> pipelineLayout;
		IDObject<AutoCleanupRenderPass> renderPass;
		uint32_t subpassIndex;
		uint32_t padding;
		GraphicsPipelineDerrivationData pipelineDerrivationData;

		GraphicsPipelineCreationData();
	};
}