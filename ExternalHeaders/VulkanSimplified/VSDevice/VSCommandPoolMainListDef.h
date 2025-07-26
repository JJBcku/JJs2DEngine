#pragma once

#include <CustomLists/IDObjectDef.h>

#include <memory>

namespace VulkanSimplified
{
	class CommandPoolMainList;
	class CommandPoolQFGroupListInternal;
}

namespace VulkanSimplified
{
	typedef std::unique_ptr<CommandPoolQFGroupListInternal> CommandPoolQFGroupPointer;
}
