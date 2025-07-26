#pragma once

#include <CustomLists/IDObjectDef.h>

#include <memory>

namespace VulkanSimplified
{
	class CommandPoolQFGroupList;
	class NIRCommandPoolInternal;
	class IRCommandPoolInternal;
}

namespace VulkanSimplified
{
	typedef std::unique_ptr<NIRCommandPoolInternal> NIRPoolPointer;
	typedef std::unique_ptr<IRCommandPoolInternal> IRPoolPointer;
}
