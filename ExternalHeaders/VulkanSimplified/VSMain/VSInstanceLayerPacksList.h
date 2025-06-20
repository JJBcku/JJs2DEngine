#pragma once

namespace VulkanSimplified
{
	struct InstanceLayerPacksList
	{
		bool debugUtils;
		bool padding[1023];

		InstanceLayerPacksList();
	};
}
