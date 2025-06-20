#pragma once

#include "VSShaderTypeFlagsDef.h"

namespace VulkanSimplified
{
	enum ShaderTypeFlagBit : ShaderTypeFlags
	{
		SHADER_TYPE_FRAGMENT = 0x1,
		SHADER_TYPE_VERTEX = 0x2,
	};
}