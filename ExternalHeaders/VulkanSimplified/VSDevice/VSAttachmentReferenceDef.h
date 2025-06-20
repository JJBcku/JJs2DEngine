#pragma once

#include <CustomLists/IDObject.h>

#include <optional>

namespace VulkanSimplified
{
	struct RenderPassAttachmentReference;
}

namespace VulkanSimplified
{
	typedef std::optional<IDObject<RenderPassAttachmentReference>> AttachmentReference;
}