#pragma once

namespace VulkanSimplified
{
	struct DeviceExtensionPacksList
	{
		bool swapchainBase;
		bool padding[1023];

		DeviceExtensionPacksList();
	};
}