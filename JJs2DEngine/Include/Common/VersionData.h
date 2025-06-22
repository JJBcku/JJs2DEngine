#pragma once

#include <stdint.h>
#include <compare>

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

		std::strong_ordering operator<=>(const VersionData& rhs) const noexcept = default;
		bool operator==(const VersionData& rhs) const noexcept = default;
	};

}
