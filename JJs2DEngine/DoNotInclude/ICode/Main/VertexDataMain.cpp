#include "MainIpch.h"
#include "../../../Include/Main/VertexDataMain.h"

#include "../../DNIData/Main/VertexDataMainInternal.h"

namespace JJs2DEngine
{
	VertexDataMain::VertexDataMain(VertexDataMainInternal& ref) : _internal(ref)
	{
	}

	VertexDataMain::~VertexDataMain()
	{
	}

	IDObject<UiVertexDataLayerVersionListInternal> VertexDataMain::AddUiLayerVersionList(size_t initialVersionCapacity, size_t addOnReserving)
	{
		return _internal.AddUiLayerVersionList(initialVersionCapacity, addOnReserving);
	}

}