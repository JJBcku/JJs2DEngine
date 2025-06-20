#pragma once

#include <CustomLists\IDObjectDef.h>

#include "VSShaderListsDef.h"

namespace VulkanSimplified
{
	class ShaderListsInternal;
}

namespace VulkanSimplified
{
	class ShaderLists
	{
	public:
		ShaderLists(ShaderListsInternal& ref);
		~ShaderLists();

		IDObject<AutoCleanupFragmentShaderModule> CreateFragmentShaderModule(const char& data, size_t dataSize, size_t addOnReservation = 0);
		IDObject<AutoCleanupVertexShaderModule> CreateVertexShaderModule(const char& data, size_t dataSize, size_t addOnReservation = 0);

	private:
		ShaderListsInternal& _internal;
	};
}