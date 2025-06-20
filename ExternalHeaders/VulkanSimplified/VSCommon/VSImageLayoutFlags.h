#pragma once

#include <stdint.h>

namespace VulkanSimplified
{
	enum class ImageLayoutFlags : uint64_t
	{
		UNDEFINED = 0x1,
		GENERAL = 0x2,
		COLOR_ATTACHMENT = 0x3,
		DEPTH_STENCIL_READ_WRITE = 0x4,
		DEPTH_STENCIL_READ_ONLY = 0x5,
		SHADER_READ_ONLY = 0x6,
		TRANSFER_SOURCE = 0x7,
		TRANSFER_DESTINATION = 0x8,
		PRESENT = 0x9
	};
}