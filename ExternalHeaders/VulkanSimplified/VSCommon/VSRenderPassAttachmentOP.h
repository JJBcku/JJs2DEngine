#pragma once

#include "VSRenderPassAttachmentOPDef.h"

namespace VulkanSimplified
{
	enum class RenderPassAttachmentLoadOP : RenderPassAttachmentOP
	{
		LOAD = 0x1,
		CLEAR = 0x2,
		IGNORE = 0x3,
	};

	enum class RenderPassAttachmentStoreOP : RenderPassAttachmentOP
	{
		STORE = 0x1,
		IGNORE = 0x2,
	};
}