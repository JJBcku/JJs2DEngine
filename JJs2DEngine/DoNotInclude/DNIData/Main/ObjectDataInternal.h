#pragma once

#include <glm/vec2.hpp>

#include <memory>

namespace JJs2DEngine
{
	struct TextureReferenceData;

	struct ObjectDataInternal
	{
		glm::vec2 texturesSizeInTile;
		glm::vec2 objectsSizeOnScreen;
		glm::vec2 objectsPositionOnScreen;
		size_t depthUNORM;
		std::shared_ptr<TextureReferenceData> textureDataPointer;

		ObjectDataInternal();
		~ObjectDataInternal();
	};
}
