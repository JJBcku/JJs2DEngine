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

		struct FragmentShaderID
		{
			ShaderTypeFlagBit type;
			IDObject<AutoCleanupFragmentShaderModule> fragmentShaderID;

			FragmentShaderID(const IDObject<AutoCleanupFragmentShaderModule>& fragmentShaderID) noexcept;
			~FragmentShaderID() = default;
		} fragmentShader;

		struct VertexShaderID
		{
			ShaderTypeFlagBit type;
			IDObject<AutoCleanupVertexShaderModule> vertexShaderID;

			VertexShaderID(const IDObject<AutoCleanupVertexShaderModule>& vertexShaderID) noexcept;
			~VertexShaderID() = default;
		} vertexShader;

		ArbitraryShaderID() noexcept;
		ArbitraryShaderID(const IDObject<AutoCleanupFragmentShaderModule>& fragmentShaderID) noexcept;
		ArbitraryShaderID(const IDObject<AutoCleanupVertexShaderModule>& vertexShaderID) noexcept;

		ArbitraryShaderID(const ArbitraryShaderID& rhs) noexcept;
		ArbitraryShaderID(ArbitraryShaderID&& rhs) noexcept;
		~ArbitraryShaderID();

		ArbitraryShaderID& operator=(const ArbitraryShaderID& rhs) noexcept;
		ArbitraryShaderID& operator=(ArbitraryShaderID&& rhs) noexcept;
	};
}