#pragma once

#include <CustomLists/IDObjectDef.h>

#include <memory>

namespace VulkanSimplified
{
	class PrimaryNIRCommandBufferInternal;
	class SecondaryNIRCommandBufferInternal;
}

namespace VulkanSimplified
{
	typedef std::unique_ptr<PrimaryNIRCommandBufferInternal> PrimaryNIRPointer;
	typedef std::unique_ptr<SecondaryNIRCommandBufferInternal> SecondaryNIRPointer;
}