#include "MainDNIpch.h"
#include "WorldLayerObjectDataInternal.h"

#include "../Common/MaxDepthValue.h"

namespace JJs2DEngine
{
	WorldLayerObjectDataInternal::WorldLayerObjectDataInternal() : texturesSizeInTile(1.0f), objectsSizeOnScreen(0.0f), objectsPositionOnScreen(0.0f),
		depthUNORM(maxDepthValue + 1), textureDataPointer()
	{
	}

	WorldLayerObjectDataInternal::~WorldLayerObjectDataInternal()
	{
	}
}