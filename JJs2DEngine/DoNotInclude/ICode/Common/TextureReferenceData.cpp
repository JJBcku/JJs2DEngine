#include "CommonIpch.h"
#include "../../../Include/Common/TextureReferenceData.h"

namespace JJs2DEngine
{
	TextureReferenceData::TextureReferenceData()
	{
		textureCoords = glm::vec2(0.0f);
		textureSize = glm::vec2(1.0f);
		textureLayer = 0;
		textureIndex = 0;

		textureIsUsed = false;
	}

	TextureReferenceData::TextureReferenceData(glm::vec2 textureCoords, glm::vec2 textureSize, uint32_t textureLayer, uint32_t textureIndex, bool textureIsUsed) :
		textureCoords(textureCoords), textureSize(textureSize), textureLayer(textureLayer), textureIndex(textureIndex), textureIsUsed(textureIsUsed)
	{
	}

	TextureReferenceData::~TextureReferenceData()
	{
	}

}