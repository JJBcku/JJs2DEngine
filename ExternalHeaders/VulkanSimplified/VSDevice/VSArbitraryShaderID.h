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

		ArbitraryShaderID() noexcept;
		ArbitraryShaderID(IDObject<AutoCleanupFragmentShaderModule> fragmentShaderID) noexcept;
		ArbitraryShaderID(IDObject<AutoCleanupVertexShaderModule> vertexShaderID) noexcept;

		ArbitraryShaderID(const ArbitraryShaderID& rhs) noexcept;
		ArbitraryShaderID(ArbitraryShaderID&& rhs) noexcept;
		~ArbitraryShaderID();

		ArbitraryShaderID& operator=(const ArbitraryShaderID& rhs) noexcept;
		ArbitraryShaderID& operator=(ArbitraryShaderID&& rhs) noexcept;
	};
}