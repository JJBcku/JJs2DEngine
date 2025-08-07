#pragma once

#include <glm/vec2.hpp>

namespace JJs2DEngine
{
	struct TextureReferenceData
	{
		glm::vec2 textureCoords;
		glm::vec2 textureSize;
		uint32_t textureLayer;
		uint32_t textureIndex;

		TextureReferenceData();
		TextureReferenceData(glm::vec2 textureCoords, glm::vec2 textureSize, uint32_t textureLayer, uint32_t textureIndex);
		~TextureReferenceData();
	};
}
