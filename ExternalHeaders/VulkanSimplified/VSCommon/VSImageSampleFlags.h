#pragma once

#include "VSImageSampleFlagsDef.h"

namespace VulkanSimplified
{
	enum ImageSampleFlagBits : ImageSampleFlags
	{
		SAMPLE_1 = 0x1,
		SAMPLE_2 = 0x2,
		SAMPLE_4 = 0x4,
		SAMPLE_8 = 0x8,
		SAMPLE_16 = 0x10,
		SAMPLE_32 = 0x20,
		SAMPLE_64 = 0x40,
	};
}