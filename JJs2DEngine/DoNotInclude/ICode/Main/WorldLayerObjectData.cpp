#include "MainIpch.h"
#include "../../../Include/Main/WorldLayerObjectData.h"

namespace JJs2DEngine
{
	WorldLayerObjectData::WorldLayerObjectData()
	{
		textureWidthInPixels = 0;
		textureHeightInPixels = 0;

		screenWidth_UNORM = 0;
		screenHeight_UNORM = 0;

		screenPositionX_SNORM = 0;
		screenPositionY_SNORM = 0;

		textureIndex = { 0, 0 };
		inPreloadedTexturesList = false;
	}

	WorldLayerObjectData::~WorldLayerObjectData()
	{
	}
}