#pragma once

namespace JJs2DEngine
{
	class WorldLayerVertexDataLayerVersion;
	class WorldLayerVertexDataLayerVersionListInternal;

	class WorldLayerVertexDataLayerVersionList
	{
	public:
		WorldLayerVertexDataLayerVersionList(WorldLayerVertexDataLayerVersionListInternal& ref);
		~WorldLayerVertexDataLayerVersionList();

		WorldLayerVertexDataLayerVersionList& operator=(const WorldLayerVertexDataLayerVersionList&) noexcept = delete;

		WorldLayerVertexDataLayerVersion GetLayersVersion(size_t versionIndex);

		const WorldLayerVertexDataLayerVersion GetLayersVersion(size_t versionIndex) const;

	private:
		WorldLayerVertexDataLayerVersionListInternal& _internal;
	};
}
