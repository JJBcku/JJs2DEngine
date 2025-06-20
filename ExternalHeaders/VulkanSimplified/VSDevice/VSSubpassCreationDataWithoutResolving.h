#pragma once

#include "VSAttachmentReferenceDef.h"

#include <CustomLists/IDObject.h>

#include <vector>
#include <stdint.h>

namespace VulkanSimplified
{
	struct SubpassCreationDataWithoutResolving
	{
		std::vector<AttachmentReference> inputAttachments;
		std::vector<AttachmentReference> colorAttachments;
		AttachmentReference depthStencilAttachment;
		std::vector<uint32_t> preserveAttachment;

		SubpassCreationDataWithoutResolving() = default;
	};
}