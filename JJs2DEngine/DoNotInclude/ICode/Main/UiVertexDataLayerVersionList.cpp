#include "MainIpch.h"
#include "../../../Include/Main/UiVertexDataLayerVersionList.h"

#include "../../DNIData/Main/UiVertexDataLayerVersionListInternal.h"

#include "../../../Include/Main/UiVertexDataLayerVersion.h"

namespace JJs2DEngine
{
	UiVertexDataLayerVersionList::UiVertexDataLayerVersionList(UiVertexDataLayerVersionListInternal& ref) : _internal(ref)
	{
	}

	UiVertexDataLayerVersionList::~UiVertexDataLayerVersionList()
	{
	}

	UiVertexDataLayerVersion UiVertexDataLayerVersionList::GetLayersVersion(size_t versionIndex)
	{
		return _internal.GetLayersVersion(versionIndex);
	}

	const UiVertexDataLayerVersion UiVertexDataLayerVersionList::GetLayersVersion(size_t versionIndex) const
	{
		return _internal.GetLayersVersion(versionIndex);
	}

}