#pragma once

#include <CustomLists/IDObject.h>

namespace JJs2DEngine
{
	class UiVertexDataLayerVersionListInternal;

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
			IDObject<UiVertexDataLayerVersionListInternal> ID;
		} UiLayerID;

		VertexLayerOrderID();
		VertexLayerOrderID(IDObject<UiVertexDataLayerVersionListInternal> ID);
	};
}