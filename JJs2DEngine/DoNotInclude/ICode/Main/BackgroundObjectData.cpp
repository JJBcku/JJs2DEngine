#include "MainIpch.h"
#include "../../../Include/Main/BackgroundObjectData.h"

namespace JJs2DEngine
{
	BackgroundObjectData::BackgroundObjectData()
	{
		textureWidthInPixels = 0;
		textureHeightInPixels = 0;

		textureIndex = { 0, 0 };
		inPreloadedTexturesList = false;
	}

	BackgroundObjectData::~BackgroundObjectData()
	{
	}

}