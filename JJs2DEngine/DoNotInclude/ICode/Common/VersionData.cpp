#include "CommonIpch.h"
#include "../../../Include/Common/VersionData.h"

namespace JJs2DEngine
{
	VersionData::VersionData()
	{
		majorVersion = 0;
		minorVersion = 0;
		patchVersion = 0;
		variantVersion = 0;
	}

	VersionData::VersionData(uint16_t majorVersion, uint16_t minorVersion, uint16_t patchVersion, uint16_t variantVersion)
	{
		this->majorVersion = majorVersion;
		this->minorVersion = minorVersion;
		this->patchVersion = patchVersion;
		this->variantVersion = variantVersion;
	}

}
