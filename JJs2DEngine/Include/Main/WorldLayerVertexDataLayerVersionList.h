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

		WorldLayerVertexDataLayerVersion GetLayersVersion(size_t versionIndex);

		const WorldLayerVertexDataLayerVersion GetLayersVersion(size_t versionIndex) const;

	private:
		WorldLayerVertexDataLayerVersionListInternal& _internal;
	};
}
