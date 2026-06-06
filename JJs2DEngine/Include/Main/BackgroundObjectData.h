#pragma once

#include "NormalizedValuesList.h"

namespace JJs2DEngine
{
	struct BackgroundObjectData
	{
		uint32_t textureWidthInPixels;
		uint32_t textureHeightInPixels;

		std::pair<size_t, size_t> textureIndex;
		bool inPreloadedTexturesList;
		uint8_t padding1;
		uint16_t padding2;
		uint32_t padding3;

		BackgroundObjectData();
		~BackgroundObjectData();
	};
}
