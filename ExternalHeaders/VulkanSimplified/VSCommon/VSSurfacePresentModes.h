#pragma once

#include "VSSurfacePresentModesDef.h"

namespace VulkanSimplified
{
	enum SurfacePresentModeBits : SurfacePresentModes
	{
		PRESENT_MODE_IMMEDIATE = 0x1,
		PRESENT_MODE_MAILBOX = 0x2,
		PRESENT_MODE_FIFO_STRICT = 0x4,
		PRESENT_MODE_FIFO_RELAXED = 0x8,
	};
}