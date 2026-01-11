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

		ShaderSpecializationData() noexcept;
		ShaderSpecializationData(const ShaderSpecializationData& rhs) noexcept;
		ShaderSpecializationData(ShaderSpecializationData&& rhs) noexcept;
		~ShaderSpecializationData();

		ShaderSpecializationData& operator=(const ShaderSpecializationData& rhs) noexcept;
		ShaderSpecializationData& operator=(ShaderSpecializationData&& rhs) noexcept;
	};

	struct ShaderStageData
	{
		IDObject<SharedShaderPipelineData> sharedData;
		ArbitraryShaderID shaderDeviceID;

		std::optional<ShaderSpecializationData> specializationData;

		ShaderStageData() noexcept;
		ShaderStageData(const ShaderStageData& rhs) noexcept;
		ShaderStageData(ShaderStageData&& rhs) noexcept;
		~ShaderStageData();

		ShaderStageData& operator=(const ShaderStageData& rhs) noexcept;
		ShaderStageData& operator=(ShaderStageData&& rhs) noexcept;
	};

	struct ViewportStatePipelineDataPoint
	{
		IDObject<PipelineViewportData> viewport;
		IDObject<PipelineScissorData> scissor;

		ViewportStatePipelineDataPoint() noexcept;
		ViewportStatePipelineDataPoint(const ViewportStatePipelineDataPoint& rhs) noexcept;
		ViewportStatePipelineDataPoint(ViewportStatePipelineDataPoint&& rhs) noexcept;
		~ViewportStatePipelineDataPoint();

		ViewportStatePipelineDataPoint& operator=(const ViewportStatePipelineDataPoint& rhs) noexcept;
		ViewportStatePipelineDataPoint& operator=(ViewportStatePipelineDataPoint&& rhs) noexcept;
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

		GraphicsPipelineDerrivationData() noexcept;
		GraphicsPipelineDerrivationData(const GraphicsPipelineDerrivationData& rhs) noexcept;
		GraphicsPipelineDerrivationData(GraphicsPipelineDerrivationData&& rhs) noexcept;
		~GraphicsPipelineDerrivationData();

		GraphicsPipelineDerrivationData& operator=(const GraphicsPipelineDerrivationData& rhs) noexcept;
		GraphicsPipelineDerrivationData& operator=(GraphicsPipelineDerrivationData&& rhs) noexcept;
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

		GraphicsPipelineCreationData() noexcept;
		GraphicsPipelineCreationData(const GraphicsPipelineCreationData& rhs) noexcept;
		GraphicsPipelineCreationData(GraphicsPipelineCreationData&& rhs) noexcept;
		~GraphicsPipelineCreationData();

		GraphicsPipelineCreationData& operator=(const GraphicsPipelineCreationData& rhs) noexcept;
		GraphicsPipelineCreationData& operator=(GraphicsPipelineCreationData&& rhs) noexcept;
	};
}