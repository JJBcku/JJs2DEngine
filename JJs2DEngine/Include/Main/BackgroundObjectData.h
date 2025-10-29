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

		BackgroundObjectData();
		~BackgroundObjectData();
	};
}
