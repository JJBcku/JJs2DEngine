#include "MainDNIpch.h"
#include "BackgroundObjectBufferData.h"

namespace JJs2DEngine
{
	BackgroundObjectBufferData::BackgroundObjectBufferData()
	{
		textureCoordinates = glm::vec2(1.0f);
		textureSize = glm::vec2(0.0f);
		textureLayer = std::numeric_limits<uint32_t>::max();
		textureIndex = std::numeric_limits<uint32_t>::max();
	}

	BackgroundObjectBufferData::~BackgroundObjectBufferData()
	{
	}

}