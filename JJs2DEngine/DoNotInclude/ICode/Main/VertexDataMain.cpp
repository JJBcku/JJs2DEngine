#include "MainIpch.h"
#include "../../../Include/Main/VertexDataMain.h"

#include "../../DNIData/Main/VertexDataMainInternal.h"

#include "../../../Include/Main/UiVertexDataLayerVersionList.h"

namespace JJs2DEngine
{
	VertexDataMain::VertexDataMain(VertexDataMainInternal& ref) : _internal(ref)
	{
	}

	VertexDataMain::~VertexDataMain()
	{
	}

	IDObject<UiVertexDataLayerVersionListPointer> VertexDataMain::AddUiLayerVersionList(const std::vector<uint64_t>& versionsMaxVerticesList, size_t addOnReserving)
	{
		return _internal.AddUiLayerVersionList(versionsMaxVerticesList, addOnReserving);
	}

	UiVertexDataLayerVersionList VertexDataMain::GetUiVertexDataLayerVersionList(IDObject<UiVertexDataLayerVersionListPointer> ID)
	{
		return _internal.GetUiVertexDataLayerVersionList(ID);
	}

	const UiVertexDataLayerVersionList VertexDataMain::GetUiVertexDataLayerVersionList(IDObject<UiVertexDataLayerVersionListPointer> ID) const
	{
		return _internal.GetUiVertexDataLayerVersionList(ID);
	}
}