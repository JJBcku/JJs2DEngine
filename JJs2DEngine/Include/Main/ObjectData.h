#pragma once

#include <stdint.h>
#include <utility>

namespace JJs2DEngine
{
	constexpr uint32_t onePointZeroUNORMValue = 0x1000000;

	struct ObjectData
	{
		uint32_t textureWidthUNORM;
		uint32_t textureHeightUNORM;

		uint32_t screenWidthUNORM;
		uint32_t screenHeightUNORM;

		uint32_t screenPositionXUNORM;
		uint32_t screenPositionYUNORM;

		std::pair<size_t, size_t> textureIndex;
		bool inPreloadedTexturesList;

		ObjectData();
		~ObjectData();
	};
}
