#pragma once

#include <stdint.h>

namespace VulkanSimplified
{
	typedef uint64_t RenderPassAttachmentOP;
	enum class RenderPassAttachmentLoadOP : RenderPassAttachmentOP;
	enum class RenderPassAttachmentStoreOP : RenderPassAttachmentOP;
}