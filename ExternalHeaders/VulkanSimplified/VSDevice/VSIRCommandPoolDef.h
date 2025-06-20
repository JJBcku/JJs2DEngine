#pragma once

#include <CustomLists/IDObjectDef.h>

#include <memory>

namespace VulkanSimplified
{
	class PrimaryIRCommandBufferInternal;
	class SecondaryIRCommandBufferInternal;
}

namespace VulkanSimplified
{
	typedef std::unique_ptr<PrimaryIRCommandBufferInternal> PrimaryIRPointer;
	typedef std::unique_ptr<SecondaryIRCommandBufferInternal> SecondaryIRPointer;
}