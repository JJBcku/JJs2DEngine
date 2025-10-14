#pragma once

#include <glm/vec2.hpp>

#include <compare>

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

		std::strong_ordering operator<=>(const TextureReferenceData& rhs) const noexcept = default;
		bool operator==(const TextureReferenceData& rhs) const noexcept = default;
	};
}
