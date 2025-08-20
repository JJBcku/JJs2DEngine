#pragma once

#include <CustomLists\IDObjectDef.h>

#include "VSDescriptorDataListsDef.h"

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
	struct DescriptorSetCombined2DArrayTextureSamplerWriteData;

	struct DescriptorSetInputAttachmentWriteData;

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
		void WriteNIFDescriptorSetCombined2DArrayTextureSamplerBindings(IDObject<AutoCleanupNIFDescriptorPool> descriptorPoolID,
			const std::vector<DescriptorSetCombined2DArrayTextureSamplerWriteData>& writeDataList);

		void WriteNIFDescriptorSetInputAttachmentBindings(IDObject<AutoCleanupNIFDescriptorPool> descriptorPoolID,
			const std::vector<DescriptorSetInputAttachmentWriteData>& writeDataList);

		void ResetNIFDescriptorPool(IDObject<AutoCleanupNIFDescriptorPool> descriptorPoolID);

		bool DeleteNIFDescriptorPool(IDObject<AutoCleanupNIFDescriptorPool> descriptorPoolID, bool throwOnIDNotFound);

		IDObject<AutoCleanupIFDescriptorPool> AddIndividualFreeingDescriptorPool(uint32_t maxTotalSetCount,
			const std::vector<std::pair<DescriptorTypeFlagBits, uint32_t>>& maxTypeCountsList, size_t addOnReserving = 0);

		std::vector<IDObject<AutoCleanupDescriptorSet>> AllocateIFDescriptorSets(
			IDObject<AutoCleanupIFDescriptorPool> descriptorPoolID,
			std::vector<IDObject<AutoCleanupDescriptorSetLayout>> descriptorSetLayoutIDs);

		void WriteIFDescriptorSetUniformBufferBindings(IDObject<AutoCleanupIFDescriptorPool> descriptorPoolID,
			const std::vector<DescriptorSetUniformBufferBindingWriteData>& writeDataList);
		void WriteIFDescriptorSetCombined2DTextureSamplerBindings(IDObject<AutoCleanupIFDescriptorPool> descriptorPoolID,
			const std::vector<DescriptorSetCombined2DTextureSamplerWriteData>& writeDataList);
		void WriteIFDescriptorSetCombined2DArrayTextureSamplerBindings(IDObject<AutoCleanupIFDescriptorPool> descriptorPoolID,
			const std::vector<DescriptorSetCombined2DArrayTextureSamplerWriteData>& writeDataList);

		void WriteIFDescriptorSetInputAttachmentBindings(IDObject<AutoCleanupNIFDescriptorPool> descriptorPoolID,
			const std::vector<DescriptorSetInputAttachmentWriteData>& writeDataList);

		std::vector<bool> FreeDescriptorSets(IDObject<AutoCleanupIFDescriptorPool> descriptorPoolID, const std::vector<IDObject<AutoCleanupDescriptorSet>>& descriptorSetsIDs,
			bool throwOnIDNotFound);
		void ResetIFDescriptorPool(IDObject<AutoCleanupIFDescriptorPool> descriptorPoolID);

		bool DeleteIFDescriptorPool(IDObject<AutoCleanupIFDescriptorPool> descriptorPoolID, bool throwOnIDNotFound);

	private:
		DescriptorDataListsInternal& _internal;
	};
}