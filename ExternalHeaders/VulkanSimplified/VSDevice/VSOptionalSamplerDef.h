#pragma once

#include "VSImageDataListsDef.h"

#include <CustomLists/IDObject.h>

#include <optional>

namespace VulkanSimplified
{
	typedef std::optional<IDObject<AutoCleanupSampler>> OptionalSampler;
}
