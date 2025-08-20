#pragma once

#include <CustomLists/IDObject.h>

#include "VSDescriptorDataListsDef.h"

#include "../VSCommon/VSImageViewGenericID.h"
#include "../VSCommon/VSImageLayoutFlags.h"

#include <vector>
#include <optional>

namespace VulkanSimplified
{
	typedef std::optional<ImageViewGenericID> OptionalGenericID;

	struct DescriptorSetInputAttachmentWriteData
	{
		IDObject<AutoCleanupDescriptorSet> descriptorSetID;
		uint32_t binding;
		uint32_t startArrayIndex;
		std::vector<std::pair<OptionalGenericID, ImageLayoutFlags>> imageDataList;

		DescriptorSetInputAttachmentWriteData();
		~DescriptorSetInputAttachmentWriteData();
	};
}