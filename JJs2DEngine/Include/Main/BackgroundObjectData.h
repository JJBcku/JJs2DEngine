#pragma once

#include "NormalizedValuesList.h"

namespace JJs2DEngine
{
	struct BackgroundObjectData
	{
		uint32_t textureWidth_UNORM;
		uint32_t textureHeight_UNORM;

		std::pair<size_t, size_t> textureIndex;
		bool inPreloadedTexturesList;

		BackgroundObjectData();
		~BackgroundObjectData();
	};
}
