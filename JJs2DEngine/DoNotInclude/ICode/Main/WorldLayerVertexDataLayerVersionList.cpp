#include "MainIpch.h"
#include "../../../Include/Main/WorldLayerVertexDataLayerVersionList.h"

#include "../../DNIData/Main/WorldLayerVertexDataLayerVersionListInternal.h"

#include "../../../Include/Main/WorldLayerVertexDataLayerVersion.h"

namespace JJs2DEngine
{
	WorldLayerVertexDataLayerVersionList::WorldLayerVertexDataLayerVersionList(WorldLayerVertexDataLayerVersionListInternal& ref) : _internal(ref)
	{
	}

	WorldLayerVertexDataLayerVersionList::~WorldLayerVertexDataLayerVersionList()
	{
	}

	WorldLayerVertexDataLayerVersion WorldLayerVertexDataLayerVersionList::GetLayersVersion(size_t versionIndex)
	{
		return _internal.GetLayersVersion(versionIndex);
	}

	const WorldLayerVertexDataLayerVersion WorldLayerVertexDataLayerVersionList::GetLayersVersion(size_t versionIndex) const
	{
		return _internal.GetLayersVersion(versionIndex);
	}
}