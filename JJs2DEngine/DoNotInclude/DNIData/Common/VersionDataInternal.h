#pragma once

#include <VulkanSimplified/VSCommon/VSVersionData.h>

#include "../../../Include/Common/VersionData.h"

namespace JJs2DEngine
{
	VersionData GetVersionData(const VS::VersionData& VSVersion);

	VS::VersionData GetVSVersionData(const VersionData& version);
}