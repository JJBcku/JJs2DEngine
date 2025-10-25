#pragma once

#include <optional>

namespace JJs2DEngine
{
	class WorldLayerVertexDataLayerVersionInternal;

	struct WorldLayerObjectData;

	class WorldLayerVertexDataLayerVersion
	{
	public:
		WorldLayerVertexDataLayerVersion(WorldLayerVertexDataLayerVersionInternal& ref);
		~WorldLayerVertexDataLayerVersion();

		std::optional<size_t> AddObject(const WorldLayerObjectData& newObjectData);

	private:
		WorldLayerVertexDataLayerVersionInternal& _internal;
	};
}