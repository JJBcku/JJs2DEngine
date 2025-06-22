#include "CommonDNIpch.h"
#include "VersionDataInternal.h"

namespace JJs2DEngine
{
	VersionData GetVersionData(const VS::VersionData& VSVersion)
	{
		VersionData ret;

		ret.majorVersion = VSVersion.GetVulkanMajorVersion();
		ret.minorVersion = VSVersion.GetVulkanMinorVersion();
		ret.patchVersion = VSVersion.GetVulkanPatchVersion();
		ret.variantVersion = VSVersion.GetVulkanVariantVersion();

		return ret;
	}

	VS::VersionData GetVSVersionData(const VersionData& version)
	{
		VS::VersionData ret;

		ret.SetVulkanVariantVersion(version.variantVersion);
		ret.SetVulkanMajorVersion(version.majorVersion);
		ret.SetVulkanMinorVersion(version.minorVersion);
		ret.SetVulkanPatchVersion(version.patchVersion);

		return ret;
	}

}