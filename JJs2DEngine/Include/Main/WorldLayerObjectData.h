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
		uint8_t padding1;
		uint16_t padding2;
		uint32_t padding3;

		WorldLayerObjectData();
		~WorldLayerObjectData();
	};
}