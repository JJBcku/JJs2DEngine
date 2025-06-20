#pragma once

#include "VSColorBlendingComponentFlagsDef.h"

namespace VulkanSimplified
{
	enum ColorBlendingComponentBits : ColorBlendingComponentFlags
	{
		COLOR_COMPONENT_R = 0x1,
		COLOR_COMPONENT_G = 0x2,
		COLOR_COMPONENT_B = 0x4,
		COLOR_COMPONENT_A = 0x8,
	};
}