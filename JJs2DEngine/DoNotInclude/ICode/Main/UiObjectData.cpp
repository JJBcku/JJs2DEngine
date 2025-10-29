#include "MainIpch.h"
#include "../../../Include/Main/UiObjectData.h"

namespace JJs2DEngine
{
	UiObjectData::UiObjectData()
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

	UiObjectData::~UiObjectData()
	{
	}

}