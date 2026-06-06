#pragma once

#include <CustomLists/IDObject.h>

namespace VulkanSimplified
{
	struct RenderPassDoubleColorClearValues;
	struct RenderPassInt64ColorClearValues;
	struct RenderPassUInt64ColorClearValues;
	struct RenderPassFloatColorClearValues;
	struct RenderPassIntColorClearValues;
	struct RenderPassUIntColorClearValues;
	struct RenderPassDepthStencilClearValues;
}

namespace VulkanSimplified
{
	enum class RenderPassClearValueIDType : uint64_t
	{
		COLOR_DOUBLE = 0x1,
		COLOR_INT64 = 0x2,
		COLOR_UINT64 = 0x4,
		COLOR_FLOAT = 0x8,
		COLOR_INT32 = 0x10,
		COLOR_UINT32 = 0x20,
		DEPTH_STENCIL = 0x40,
	};

	union RenderPassClearValueID
	{
		RenderPassClearValueIDType type;

		struct DoubleColorID
		{
			RenderPassClearValueIDType type;
			IDObject<RenderPassDoubleColorClearValues> ID;

			DoubleColorID(const IDObject<RenderPassDoubleColorClearValues>& ID) noexcept;
			~DoubleColorID() = default;
		} doubleColorID;

		struct Int64ColorID
		{
			RenderPassClearValueIDType type;
			IDObject<RenderPassInt64ColorClearValues> ID;

			Int64ColorID(const IDObject<RenderPassInt64ColorClearValues>& ID) noexcept;
			~Int64ColorID() = default;
		} int64ColorID;

		struct Uint64ColorID
		{
			RenderPassClearValueIDType type;
			IDObject<RenderPassUInt64ColorClearValues> ID;

			Uint64ColorID(const IDObject<RenderPassUInt64ColorClearValues>& ID) noexcept;
			~Uint64ColorID() = default;
		} uint64ColorID;

		struct FloatColorID
		{
			RenderPassClearValueIDType type;
			IDObject<RenderPassFloatColorClearValues> ID;

			FloatColorID(const IDObject<RenderPassFloatColorClearValues>& ID) noexcept;
			~FloatColorID() = default;
		} floatColorID;

		struct Int32ColorID
		{
			RenderPassClearValueIDType type;
			IDObject<RenderPassIntColorClearValues> ID;

			Int32ColorID(const IDObject<RenderPassIntColorClearValues>& ID) noexcept;
			~Int32ColorID() = default;
		} int32ColorID;

		struct Uint32ColorID
		{
			RenderPassClearValueIDType type;
			IDObject<RenderPassUIntColorClearValues> ID;

			Uint32ColorID(const IDObject<RenderPassUIntColorClearValues>& ID) noexcept;
			~Uint32ColorID() = default;
		} uint32ColorID;

		struct DepthStencilID
		{
			RenderPassClearValueIDType type;
			IDObject<RenderPassDepthStencilClearValues> ID;

			DepthStencilID(const IDObject<RenderPassDepthStencilClearValues>& ID) noexcept;
			~DepthStencilID() = default;
		} depthStencilID;

		RenderPassClearValueID() noexcept;
		RenderPassClearValueID(const IDObject<RenderPassDoubleColorClearValues>& ID) noexcept;
		RenderPassClearValueID(const IDObject<RenderPassInt64ColorClearValues>& ID) noexcept;
		RenderPassClearValueID(const IDObject<RenderPassUInt64ColorClearValues>& ID) noexcept;

		RenderPassClearValueID(const IDObject<RenderPassFloatColorClearValues>& ID) noexcept;
		RenderPassClearValueID(const IDObject<RenderPassIntColorClearValues>& ID) noexcept;
		RenderPassClearValueID(const IDObject<RenderPassUIntColorClearValues>& ID) noexcept;

		RenderPassClearValueID(const IDObject<RenderPassDepthStencilClearValues>& ID) noexcept;
		RenderPassClearValueID(const RenderPassClearValueID& rhs) noexcept;
		~RenderPassClearValueID();

		RenderPassClearValueID& operator=(const RenderPassClearValueID& rhs) noexcept;
	};
}