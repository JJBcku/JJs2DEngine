#pragma once

#include "NormalizedValuesList.h"

#include <utility>

namespace JJs2DEngine
{
	struct WorldLayerObjectData
	{
		uint32_t textureWidth_UNORM;
		uint32_t textureHeight_UNORM;

		uint32_t screenWidth_UNORM;
		uint32_t screenHeight_UNORM;

		int32_t screenPositionX_SNORM;
		int32_t screenPositionY_SNORM;

		std::pair<size_t, size_t> textureIndex;
		bool inPreloadedTexturesList;

		WorldLayerObjectData();
		~WorldLayerObjectData();
	};
}