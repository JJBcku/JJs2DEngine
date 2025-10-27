#pragma once

#include <glm\vec2.hpp>

namespace JJs2DEngine
{
	struct BackgroundObjectBufferData
	{
		glm::vec2 textureCoordinates;
		glm::vec2 textureSize;
		uint32_t textureLayer;
		uint32_t textureIndex;

		BackgroundObjectBufferData();
		~BackgroundObjectBufferData();
	};
}
