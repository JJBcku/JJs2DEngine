#include "MainIpch.h"
#include "../../../Include/Main/VertexLayerOrderID.h"

namespace JJs2DEngine
{
	VertexLayerOrderID::VertexLayerOrderID()
	{
		type = VertexLayerOrderIDType::UNSET;
	}

	VertexLayerOrderID::VertexLayerOrderID(IDObject<UiVertexDataLayerVersionListPointer> ID)
	{
		uiLayerID.type = VertexLayerOrderIDType::UI_LAYER;
		uiLayerID.ID = ID;
	}

	VertexLayerOrderID::VertexLayerOrderID(IDObject<WorldLayerVertexDataLayerVersionListPointer> ID)
	{
		worldLayerID.type = VertexLayerOrderIDType::WORLD_LAYER;
		worldLayerID.ID = ID;
	}

}