#include "MainIpch.h"
#include "../../../Include/Main/ObjectData.h"

namespace JJs2DEngine
{
	ObjectData::ObjectData()
	{
		textureWidthUNORM = 0;
		textureHeightUNORM = 0;

		screenWidthUNORM = 0;
		screenHeightUNORM = 0;

		screenPositionXSNORM = 0;
		screenPositionYSNORM = 0;

		textureIndex = { 0, 0 };
		inPreloadedTexturesList = false;
	}

	ObjectData::~ObjectData()
	{
	}

}