#pragma once

#include "VSCompositeAlphaFlagsDef.h"

namespace VulkanSimplified
{
	enum CompositeAlphaFlagBits : CompositeAlphaFlags
	{
		COMPOSITE_ALPHA_OPAQUE = 0x1,
		COMPOSITE_ALPHA_PRE_MULTIPLIED = 0x2,
		COMPOSITE_ALPHA_POST_MULTIPLIED = 0x4,
	};
}