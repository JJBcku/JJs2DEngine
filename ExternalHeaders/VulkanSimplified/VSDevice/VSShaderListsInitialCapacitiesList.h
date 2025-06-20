#pragma once

namespace VulkanSimplified
{
	struct ShaderListsInitialCapacitiesList
	{
		size_t fragmentShadersListInitialCapacity;
		size_t vertexShadersListInitialCapacity;

		ShaderListsInitialCapacitiesList();
	};
}