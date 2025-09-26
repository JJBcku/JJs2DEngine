#pragma once

#include <stdint.h>
#include <utility>

namespace JJs2DEngine
{
	constexpr uint32_t onePointZeroUNORMValue = 0x1000000;
	constexpr uint32_t onePointZeroSNORMValue = 0x1000000;
	constexpr int32_t minusOnePointZeroSNORMValue = 0xFF000000;

	struct ObjectData
	{
		uint32_t textureWidthUNORM;
		uint32_t textureHeightUNORM;

		uint32_t screenWidthUNORM;
		uint32_t screenHeightUNORM;

		int32_t screenPositionXSNORM;
		int32_t screenPositionYSNORM;

		std::pair<size_t, size_t> textureIndex;
		bool inPreloadedTexturesList;

		ObjectData();
		~ObjectData();
	};
}
