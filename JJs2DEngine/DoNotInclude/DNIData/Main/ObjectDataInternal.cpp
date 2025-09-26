#include "MainDNIpch.h"
#include "ObjectDataInternal.h"

#include "../Common/MaxDepthValue.h"

namespace JJs2DEngine
{
	ObjectDataInternal::ObjectDataInternal() : texturesSizeInTile(1.0f), objectsSizeOnScreen(0.0f), objectsPositionOnScreen(0.0f), textureDataPointer(), depthUNORM(maxDepthValue + 1)
	{
	}

	ObjectDataInternal::~ObjectDataInternal()
	{
	}

}