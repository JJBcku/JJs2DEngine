#pragma once

#include "NormalizedValuesList.h"

#include <utility>

namespace JJs2DEngine
{
	struct WorldLayerObjectData
	{
		uint32_t textureWidthInPixels;
		uint32_t textureHeightInPixels;

		uint32_t worldWidth_UNORM;
		uint32_t worldHeight_UNORM;

		int32_t worldPositionX_SNORM;
		int32_t worldPositionY_SNORM;

		std::pair<size_t, size_t> textureIndex;
		bool inPreloadedTexturesList;

		WorldLayerObjectData();
		~WorldLayerObjectData();
	};
}