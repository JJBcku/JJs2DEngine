#pragma once

#include "../VSCommon/VSAccessFlagsDef.h"

namespace VulkanSimplified
{
	struct GlobalMemoryBarrierData
	{
		AccessFlags srcAccess;
		AccessFlags dstAccess;

		GlobalMemoryBarrierData();
	};
}