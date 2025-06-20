#pragma once

#include <memory>

namespace VulkanSimplified
{
	class WindowInternal;
}

namespace VulkanSimplified
{
	typedef std::unique_ptr<WindowInternal> WindowPointer;
}