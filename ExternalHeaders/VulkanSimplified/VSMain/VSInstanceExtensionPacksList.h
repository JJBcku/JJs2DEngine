#pragma once

namespace VulkanSimplified
{
	struct InstanceExtensionPacksList
	{
		bool sdlRequiredExtensions;
		bool debugUtils;
		bool padding[1022];

		InstanceExtensionPacksList();
	};
}
