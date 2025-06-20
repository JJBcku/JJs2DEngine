#pragma once

#include <stdint.h>
#include <compare>

namespace VulkanSimplified
{
	class VersionData
	{
	public:
		VersionData(uint16_t variant = 0, uint16_t major = 0, uint16_t minor = 0, uint16_t patch = 0);
		VersionData(uint32_t vulkanApiVersion);

		std::strong_ordering operator<=>(const VersionData& rhs) const = default;
		bool operator==(const VersionData& rhs) const = default;

		uint32_t GetVulkanApiCompatibleVersion() const;

		uint16_t GetVulkanVariantVersion() const;
		uint16_t GetVulkanMajorVersion() const;
		uint16_t GetVulkanMinorVersion() const;
		uint16_t GetVulkanPatchVersion() const;

		void SetVulkanVariantVersion(uint16_t variant);
		void SetVulkanMajorVersion(uint16_t major);
		void SetVulkanMinorVersion(uint16_t minor);
		void SetVulkanPatchVersion(uint16_t patch);

	private:
		uint16_t _variant;
		uint16_t _major;
		uint16_t _minor;
		uint16_t _patch;
	};
}