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

	void WorldLayerVertexDataLayerVersion::ChangeObjectsTexture(bool inPreloadedTexturesList, std::pair<size_t, size_t> newTextureID, size_t objectsIndex)
	{
		_internal.ChangeObjectsTexture(inPreloadedTexturesList, newTextureID, objectsIndex);
	}

}