#pragma once

#include "NormalizedValuesList.h"

#include <utility>

namespace JJs2DEngine
{
	struct UiObjectData
	{
		uint32_t textureWidthInPixels;
		uint32_t textureHeightInPixels;

		uint32_t screenWidth_UNORM;
		uint32_t screenHeight_UNORM;

		int32_t screenPositionX_SNORM;
		int32_t screenPositionY_SNORM;

		std::pair<size_t, size_t> textureIndex;
		bool inPreloadedTexturesList;
		uint8_t padding1;
		uint16_t padding2;
		uint32_t padding3;

		UiObjectData();
		~UiObjectData();
	};
}
