#pragma once

#include <CustomLists/IDObjectDef.h>

#include <memory>

namespace VulkanSimplified
{
	class CommandPoolQFGroupListInternal;
}

namespace VulkanSimplified
{
	typedef std::unique_ptr<CommandPoolQFGroupListInternal> CommandPoolQFGroupPointer;
}