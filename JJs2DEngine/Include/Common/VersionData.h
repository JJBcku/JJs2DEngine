#pragma once

#include <stdint.h>

namespace JJs2DEngine
{
	struct VersionData
	{
		uint16_t majorVersion;
		uint16_t minorVersion;
		uint16_t patchVersion;
		uint16_t variantVersion;

		VersionData();
		VersionData(uint16_t majorVersion, uint16_t minorVersion, uint16_t patchVersion, uint16_t variantVersion);
	};

	static VersionData EngineVersion = VersionData(0, 0, 2, 0);
}
