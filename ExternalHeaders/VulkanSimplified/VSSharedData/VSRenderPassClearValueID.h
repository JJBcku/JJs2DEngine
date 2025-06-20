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
		COLOR_INT = 0x10,
		COLOR_UINT = 0x20,
		DEPTH_STENCIL = 0x40,
	};

	union RenderPassClearValueID
	{
		RenderPassClearValueIDType type;

		struct
		{
			RenderPassClearValueIDType type;
			IDObject<RenderPassDoubleColorClearValues> ID;
		} doubleColorID;

		struct
		{
			RenderPassClearValueIDType type;
			IDObject<RenderPassInt64ColorClearValues> ID;
		} int64ColorID;

		struct
		{
			RenderPassClearValueIDType type;
			IDObject<RenderPassUInt64ColorClearValues> ID;
		} uint64ColorID;

		struct
		{
			RenderPassClearValueIDType type;
			IDObject<RenderPassFloatColorClearValues> ID;
		} floatColorID;

		struct
		{
			RenderPassClearValueIDType type;
			IDObject<RenderPassIntColorClearValues> ID;
		} intColorID;

		struct
		{
			RenderPassClearValueIDType type;
			IDObject<RenderPassUIntColorClearValues> ID;
		} uintColorID;

		struct
		{
			RenderPassClearValueIDType type;
			IDObject<RenderPassDepthStencilClearValues> ID;
		} depthStencilID;

		RenderPassClearValueID();
		RenderPassClearValueID(IDObject<RenderPassDoubleColorClearValues> ID);
		RenderPassClearValueID(IDObject<RenderPassInt64ColorClearValues> ID);
		RenderPassClearValueID(IDObject<RenderPassUInt64ColorClearValues> ID);

		RenderPassClearValueID(IDObject<RenderPassFloatColorClearValues> ID);
		RenderPassClearValueID(IDObject<RenderPassIntColorClearValues> ID);
		RenderPassClearValueID(IDObject<RenderPassUIntColorClearValues> ID);

		RenderPassClearValueID(IDObject<RenderPassDepthStencilClearValues> ID);
	};
}