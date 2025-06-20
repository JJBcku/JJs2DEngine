#pragma once

#include <CustomLists/IDObject.h>

#include "../VSCommon/VSShaderTypeFlags.h"

namespace VulkanSimplified
{
	class AutoCleanupFragmentShaderModule;
	class AutoCleanupVertexShaderModule;
}

namespace VulkanSimplified
{
	union ArbitraryShaderID
	{
	public:
		ShaderTypeFlagBit type;

		struct
		{
			ShaderTypeFlagBit type;
			IDObject<AutoCleanupFragmentShaderModule> fragmentShaderID;
		} fragmentShader;

		struct
		{
			ShaderTypeFlagBit type;
			IDObject<AutoCleanupVertexShaderModule> vertexShaderID;
		} vertexShader;

		ArbitraryShaderID();
		ArbitraryShaderID(IDObject<AutoCleanupFragmentShaderModule> fragmentShaderID);
		ArbitraryShaderID(IDObject<AutoCleanupVertexShaderModule> vertexShaderID);
	};
}