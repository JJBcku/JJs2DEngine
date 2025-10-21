#pragma once

#include <CustomLists\IDObjectDef.h>

#include "VSSharedPipelineDataListsDef.h"

#include <vector>
#include <stdint.h>
#include <string>
#include <optional>

namespace VulkanSimplified
{
	class SharedPipelineDataListsInternal;
}

namespace VulkanSimplified
{
	enum ShaderTypeFlagBit : ShaderTypeFlags;
	enum ImageSampleFlagBits : ImageSampleFlags;

	typedef uint64_t ColorBlendingComponentFlags;

	enum class VertexBindingInputRate : uint64_t;
	enum class PipelinePrimitiveTopology : uint64_t;
	enum class PipelinePolygonMode : uint64_t;
	enum class DepthUsage : uint64_t;
	enum class CompareOperationsType : uint64_t;
	enum class ColorBlendingPreset : uint64_t;

	struct DataFormatSetIndependentID;

	class SharedPipelineDataList
	{
	public:
		SharedPipelineDataList(SharedPipelineDataListsInternal& ref);
		~SharedPipelineDataList();

		IDObject<ShaderSpecializationElement> AddShaderSpecializationElement(uint32_t constantID, uint32_t dataOffset, size_t dataSize,
			size_t addOnReserving = 0);
		IDObject<SharedShaderPipelineData> AddSharedShaderPipelineData(std::string entryPointName, ShaderTypeFlagBit shaderType,
			size_t addOnReserving = 0);

		IDObject<VertexAttributeDescriptionData> AddVertexAttributeDescriptionData(uint32_t offset, DataFormatSetIndependentID format,
			size_t addOnReserving = 0);
		IDObject<VertexBindingDescriptionData> AddVertexBindingData(uint32_t stride, VertexBindingInputRate inputRate,
			const std::vector<IDObject<VertexAttributeDescriptionData>>& vertexAttributeIDs, size_t addOnReserving = 0);
		IDObject<VertexInputSharedPipelineData> AddVertexInputSharedPipelineData(
			const std::vector<IDObject<VertexBindingDescriptionData>>& bindings, size_t addOnReserving = 0);

		IDObject<PipelineScissorData> AddPipelineScissorData(int32_t offsetX, int32_t offsetY, uint32_t width, uint32_t height, size_t addOnReserving = 0);
		IDObject<PipelineViewportData> AddPipelineViewportData(uint32_t startX, uint32_t startY, uint32_t width, uint32_t height,
			float minDepth, float maxDepth, size_t addOnReserving = 0);

		IDObject<PipelineInputAssemblyData> AddPipelineInputAssemblyData(PipelinePrimitiveTopology topology, bool primitiveRestartEnable,
			size_t addOnReserving = 0);
		IDObject<PipelineRasterizationData> AddPipelineRasterizationData(PipelinePolygonMode polygonMode, bool cullPolygonBacks,
			bool frontClockwise, size_t addOnReserving = 0);
		IDObject<PipelineMultisampleData> AddPipelineMultisampleData(ImageSampleFlagBits samplingSetting, std::optional<uint32_t> minSampleShading,
			size_t addOnReserving = 0);
		IDObject<PipelineDepthStencilStateData> AddPipelineDepthStencilStateData(DepthUsage depthUsage,
			CompareOperationsType compareOp, float minDepth, float maxDepth, size_t addOnReserving = 0);
		IDObject<PipelineColorBlendAttachment> AddPipelineColorBlendAttachment(ColorBlendingComponentFlags blendingComponents,
			ColorBlendingPreset blendingPreset, size_t addOnReserving = 0);

		IDObject<PushConstantData> AddPushConstantData(ShaderTypeFlagBit shaderType, uint32_t offset, uint32_t size, size_t addOnReserving = 0);

		IDObject<ShaderSpecializationElement> AddUniqueShaderSpecializationElement(uint32_t constantID, uint32_t dataOffset, size_t dataSize,
			size_t addOnReserving = 0);
		IDObject<SharedShaderPipelineData> AddUniqueSharedShaderPipelineData(std::string entryPointName, ShaderTypeFlagBit shaderType,
			size_t addOnReserving = 0);

		IDObject<VertexAttributeDescriptionData> AddUniqueVertexAttributeDescriptionData(uint32_t offset, DataFormatSetIndependentID format,
			size_t addOnReserving = 0);
		IDObject<VertexBindingDescriptionData> AddUniqueVertexBindingData(uint32_t stride, VertexBindingInputRate inputRate,
			const std::vector<IDObject<VertexAttributeDescriptionData>>& vertexAttributeIDs, size_t addOnReserving = 0);
		IDObject<VertexInputSharedPipelineData> AddUniqueVertexInputSharedPipelineData(
			const std::vector<IDObject<VertexBindingDescriptionData>>& bindings, size_t addOnReserving = 0);

		IDObject<PipelineScissorData> AddUniquePipelineScissorData(int32_t offsetX, int32_t offsetY, uint32_t width, uint32_t height,
			size_t addOnReserving = 0);
		IDObject<PipelineViewportData> AddUniquePipelineViewportData(uint32_t startX, uint32_t startY, uint32_t width, uint32_t height,
			float minDepth, float maxDepth, size_t addOnReserving = 0);

		IDObject<PipelineInputAssemblyData> AddUniquePipelineInputAssemblyData(PipelinePrimitiveTopology topology,
			bool primitiveRestartEnable, size_t addOnReserving);
		IDObject<PipelineRasterizationData> AddUniquePipelineRasterizationData(PipelinePolygonMode polygonMode, bool cullPolygonBacks,
			bool frontClockwise, size_t addOnReserving = 0);
		IDObject<PipelineMultisampleData> AddUniquePipelineMultisampleData(ImageSampleFlagBits samplingSetting, std::optional<uint32_t> minSampleShading,
			size_t addOnReserving = 0);
		IDObject<PipelineDepthStencilStateData> AddUniquePipelineDepthStencilStateData(DepthUsage depthUsage,
			CompareOperationsType compareOp, float minDepth, float maxDepth, size_t addOnReserving = 0);
		IDObject<PipelineColorBlendAttachment> AddUniquePipelineColorBlendAttachment(ColorBlendingComponentFlags blendingComponents,
			ColorBlendingPreset blendingPreset, size_t addOnReserving = 0);

		IDObject<PushConstantData> AddUniquePushConstantData(ShaderTypeFlagBit shaderType, uint32_t offset, uint32_t size,
			size_t addOnReserving = 0);

	private:
		SharedPipelineDataListsInternal& _internal;
	};
}