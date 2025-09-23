#pragma once

#include <CustomLists/IDObject.h>

#include "VertexDataMainDef.h"

namespace JJs2DEngine
{
	enum class VertexLayerOrderIDType : uint64_t
	{
		UNSET = 0,
		UI_LAYER = 1,
	};

	union VertexLayerOrderID
	{
		VertexLayerOrderIDType type;
		struct
		{
			VertexLayerOrderIDType type;
			IDObject<UiVertexDataLayerVersionListPointer> ID;
		} UiLayerID;

		VertexLayerOrderID();
		VertexLayerOrderID(IDObject<UiVertexDataLayerVersionListPointer> ID);
	};
}