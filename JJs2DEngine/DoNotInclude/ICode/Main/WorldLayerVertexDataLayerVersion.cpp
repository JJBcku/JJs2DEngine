#include "MainIpch.h"
#include "../../../Include/Main/WorldLayerVertexDataLayerVersion.h"

#include "../../DNIData/Main/WorldLayerVertexDataLayerVersionInternal.h"

namespace JJs2DEngine
{
	WorldLayerVertexDataLayerVersion::WorldLayerVertexDataLayerVersion(WorldLayerVertexDataLayerVersionInternal& ref) : _internal(ref)
	{
	}

	WorldLayerVertexDataLayerVersion::~WorldLayerVertexDataLayerVersion()
	{
	}

	std::optional<size_t> WorldLayerVertexDataLayerVersion::AddObject(const WorldLayerObjectData& newObjectData)
	{
		return _internal.AddObject(newObjectData);
	}
}