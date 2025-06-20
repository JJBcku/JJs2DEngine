#pragma once

#include <CustomLists\IDObjectDef.h>

#include "VSDescriptorDataListDef.h"

#include "../VSCommon/VSDescriptorTypeFlagsDef.h"

namespace VulkanSimplified
{
	class DescriptorDataListsInternal;

	struct DescriptorSetLayoutBindingData;
}

namespace VulkanSimplified
{
	struct DescriptorSetUniformBufferBindingWriteData;
	struct DescriptorSetCombined2DTextureSamplerWriteData;

	class DescriptorDataLists
	{
	public:
		DescriptorDataLists(DescriptorDataListsInternal& ref);
		~DescriptorDataLists();

		IDObject<AutoCleanupDescriptorSetLayout> AddDescriptorSetLayout(uint32_t firstBinding,
			const std::vector<DescriptorSetBindingFullData>& descriptorSetLayoutBindings, size_t addOnReserving = 0);

		IDObject<AutoCleanupNIFDescriptorPool> AddNoIndividualFreeingDescriptorPool(uint32_t maxTotalSetCount,
			const std::vector<std::pair<DescriptorTypeFlagBits, uint32_t>>& maxTypeCountsList, size_t addOnReserving = 0);

		std::vector<IDObject<AutoCleanupDescriptorSet>> AllocateNIFDescriptorSets(
			IDObject<AutoCleanupNIFDescriptorPool> descriptorPoolID,
			std::vector<IDObject<AutoCleanupDescriptorSetLayout>> descriptorSetLayoutIDs);

		void WriteNIFDescriptorSetUniformBufferBindings(IDObject<AutoCleanupNIFDescriptorPool> descriptorPoolID,
			const std::vector<DescriptorSetUniformBufferBindingWriteData>& writeDataList);
		void WriteNIFDescriptorSetCombined2DTextureSamplerBindings(IDObject<AutoCleanupNIFDescriptorPool> descriptorPoolID,
			const std::vector<DescriptorSetCombined2DTextureSamplerWriteData>& writeDataList);

		IDObject<AutoCleanupIFDescriptorPool> AddIndividualFreeingDescriptorPool(uint32_t maxTotalSetCount,
			const std::vector<std::pair<DescriptorTypeFlagBits, uint32_t>>& maxTypeCountsList, size_t addOnReserving = 0);

		std::vector<IDObject<AutoCleanupDescriptorSet>> AllocateIFDescriptorSets(
			IDObject<AutoCleanupIFDescriptorPool> descriptorPoolID,
			std::vector<IDObject<AutoCleanupDescriptorSetLayout>> descriptorSetLayoutIDs);

		void WriteIFDescriptorSetUniformBufferBindings(IDObject<AutoCleanupIFDescriptorPool> descriptorPoolID,
			const std::vector<DescriptorSetUniformBufferBindingWriteData>& writeDataList);
		void WriteIFDescriptorSetCombined2DTextureSamplerBindings(IDObject<AutoCleanupIFDescriptorPool> descriptorPoolID,
			const std::vector<DescriptorSetCombined2DTextureSamplerWriteData>& writeDataList);

	private:
		DescriptorDataListsInternal& _internal;
	};
}