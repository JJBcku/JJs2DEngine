#pragma once

#include <stdint.h>
#include <utility>

namespace JJs2DEngine
{
	constexpr uint32_t onePointZeroUNORMValue = 0x1000000;
	constexpr uint32_t onePointZeroSNORMValue = 0x1000000;
	constexpr int32_t minusOnePointZeroSNORMValue = 0xFF000000;

	struct UiObjectData
	{
		uint32_t textureWidth_UNORM;
		uint32_t textureHeight_UNORM;

		uint32_t screenWidth_UNORM;
		uint32_t screenHeight_UNORM;

		int32_t screenPositionX_SNORM;
		int32_t screenPositionY_SNORM;

		std::pair<size_t, size_t> textureIndex;
		bool inPreloadedTexturesList;

		UiObjectData();
		~UiObjectData();
	};
}
