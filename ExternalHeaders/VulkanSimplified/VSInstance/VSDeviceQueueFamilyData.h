#pragma once

#include <Miscellaneous/Bool64Def.h>

#include <stdint.h>

namespace VulkanSimplified
{
	typedef uint32_t QueueTypeFlags;

	enum QueueTypeFlagBits : QueueTypeFlags
	{
		QUEUE_TYPE_GRAPHICS = 0x1,
		QUEUE_TYPE_COMPUTE = 0x2,
		QUEUE_TYPE_TRANSFER = 0x4,
		QUEUE_TYPE_SPARSE_BINDING = 0x8,
		QUEUE_TYPE_PROTECTED = 0x10,
		QUEUE_TYPE_VIDEO_DECODE = 0x20,
		QUEUE_TYPE_VIDEO_ENCODE = 0x40,
		QUEUE_TYPE_OPTICAL_FLOW = 0x100,
	};

	struct QueueFamilyData
	{
		QueueTypeFlags queueTypes;
		uint32_t queueCount;
		uint32_t timespampValidBits;
		uint32_t minImageTransferGranularityWidth;
		uint32_t minImageTransferGranularityHeight;
		uint32_t minImageTransferGranularityDepth;

		Misc::Bool64 presentationSupport;

		QueueFamilyData();
		~QueueFamilyData();
	};
}