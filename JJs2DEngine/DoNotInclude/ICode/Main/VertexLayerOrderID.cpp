#include "MainIpch.h"
#include "../../../Include/Main/VertexLayerOrderID.h"

namespace JJs2DEngine
{
	VertexLayerOrderID::VertexLayerOrderID()
	{
		type = VertexLayerOrderIDType::UNSET;
	}

	VertexLayerOrderID::VertexLayerOrderID(IDObject<UiVertexDataLayerVersionListInternal> ID)
	{
		UiLayerID.type = VertexLayerOrderIDType::UI_LAYER;
		UiLayerID.ID = ID;
	}

}