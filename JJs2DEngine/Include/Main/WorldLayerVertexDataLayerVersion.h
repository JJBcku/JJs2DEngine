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

		WorldLayerVertexDataLayerVersion& operator=(const WorldLayerVertexDataLayerVersion&) noexcept = delete;

		std::optional<size_t> AddObject(const WorldLayerObjectData& newObjectData);

		void ChangeObjectsTexture(bool inPreloadedTexturesList, std::pair<size_t, size_t> newTextureID, size_t objectsIndex);

	private:
		WorldLayerVertexDataLayerVersionInternal& _internal;
	};
}