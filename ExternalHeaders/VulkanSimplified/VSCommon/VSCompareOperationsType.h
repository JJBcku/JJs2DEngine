#pragma once

#include "VSCompareOperationsTypeDef.h"

namespace VulkanSimplified
{
	enum class CompareOperationsType : uint64_t
	{
		COMPARE_OPERATION_NEVER = 1,
		COMPARE_OPERATION_ALWAYS = 2,
		COMPARE_OPERATION_EQUAL = 3,
		COMPARE_OPERATION_LESS = 4,
		COMPARE_OPERATION_LESS_OR_EQUAL = 5,
		COMPARE_OPERATION_GREATER = 6,
		COMPARE_OPERATION_GREATER_OR_EQUAL = 7,
		COMPARE_OPERATION_NOT_EQUAL = 8,
	};
}