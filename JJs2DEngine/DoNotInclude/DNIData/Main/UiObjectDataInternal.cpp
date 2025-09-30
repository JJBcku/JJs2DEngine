#include "MainDNIpch.h"
#include "UiObjectDataInternal.h"

#include "../Common/MaxDepthValue.h"

namespace JJs2DEngine
{
	UiObjectDataInternal::UiObjectDataInternal() : texturesSizeInTile(1.0f), objectsSizeOnScreen(0.0f), objectsPositionOnScreen(0.0f), textureDataPointer(), depthUNORM(maxDepthValue + 1)
	{
	}

	UiObjectDataInternal::~UiObjectDataInternal()
	{
	}

}