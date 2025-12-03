#pragma once

#include <stdint.h>
#include <string>

#include "../Common/VersionData.h"

namespace JJs2DEngine
{
	struct MainInitializationData
	{
		std::string appName;
		std::string appVariantName;
		VersionData appVersion;

		VersionData minVulkanVersion;
		VersionData maxVulkanVersion;

		std::string readOnlyDataFolder;
		std::string writenToDataFolder;

		MainInitializationData();
	};
}