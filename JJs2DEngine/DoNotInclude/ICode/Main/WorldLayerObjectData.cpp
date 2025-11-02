#include "MainIpch.h"
#include "../../../Include/Main/WorldLayerObjectData.h"

namespace JJs2DEngine
{
	WorldLayerObjectData::WorldLayerObjectData()
	{
		textureWidthInPixels = 0;
		textureHeightInPixels = 0;

		worldWidth_UNORM = 0;
		worldHeight_UNORM = 0;

		worldPositionX_SNORM = 0;
		worldPositionY_SNORM = 0;

		textureIndex = { 0, 0 };
		inPreloadedTexturesList = false;
	}

	WorldLayerObjectData::~WorldLayerObjectData()
	{
	}
}