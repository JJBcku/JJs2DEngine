#include "MainIpch.h"
#include "../../../Include/Main/BackgroundObjectData.h"

namespace JJs2DEngine
{
	BackgroundObjectData::BackgroundObjectData()
	{
		textureWidth_UNORM = 0;
		textureHeight_UNORM = 0;

		textureIndex = { 0, 0 };
		inPreloadedTexturesList = false;
	}

	BackgroundObjectData::~BackgroundObjectData()
	{
	}

}