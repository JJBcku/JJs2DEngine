#pragma once

#include <glm\vec2.hpp>

#include <memory>

namespace JJs2DEngine
{
	struct TextureReferenceData;

	struct BackgroundObjectDataInternal
	{
		glm::vec2 inTextureSize;
		std::vector<std::shared_ptr<TextureReferenceData>> textureReference;

		BackgroundObjectDataInternal();
		~BackgroundObjectDataInternal();
	};
}
