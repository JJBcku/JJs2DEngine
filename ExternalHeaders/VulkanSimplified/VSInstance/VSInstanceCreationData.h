#pragma once

#include "../VSMain/VSInstanceExtensionPacksList.h"
#include "../VSMain/VSInstanceLayerPacksList.h"
#include "../VSCommon/VSVersionData.h"

namespace VulkanSimplified
{
	struct InstanceCreationData
	{
		std::string appName;
		std::string appVariantName;
		VersionData appVersion;
		std::string engineName;
		VersionData engineVersion;

		VersionData usedVulkanApiVersion;
		InstanceExtensionPacksList enabledExtensionPacks;
		InstanceLayerPacksList enabledLayerPacks;

		InstanceCreationData() = default;
	};
}