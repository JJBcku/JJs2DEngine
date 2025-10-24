#pragma once

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include <stdint.h>

namespace JJs2DEngine
{
	struct WorldLayerObjectBufferData
	{
		glm::vec2 texCoords;
		glm::vec2 texSize;
		glm::vec4 pos;
		glm::vec2 size;
		uint32_t texLayer;
		uint32_t texIndex;

		WorldLayerObjectBufferData();
		~WorldLayerObjectBufferData();
	};
}