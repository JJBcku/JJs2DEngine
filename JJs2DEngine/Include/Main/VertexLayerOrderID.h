#pragma once

#include <CustomLists/IDObject.h>

#include "VertexDataMainDef.h"

namespace JJs2DEngine
{
	enum class VertexLayerOrderIDType : uint64_t
	{
		UNSET = 0,
		UI_LAYER = 1,
		WORLD_LAYER = 2,
	};

	union VertexLayerOrderID
	{
		VertexLayerOrderIDType type;
		struct
		{
			VertexLayerOrderIDType type;
			IDObject<UiVertexDataLayerVersionListPointer> ID;
		} uiLayerID;

		struct
		{
			VertexLayerOrderIDType type;
			IDObject<WorldLayerVertexDataLayerVersionListPointer> ID;
		} worldLayerID;

		VertexLayerOrderID();
		VertexLayerOrderID(IDObject<UiVertexDataLayerVersionListPointer> ID);
		VertexLayerOrderID(IDObject<WorldLayerVertexDataLayerVersionListPointer> ID);
	};
}