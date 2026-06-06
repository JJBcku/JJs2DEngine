#pragma once

namespace JJs2DEngine
{
	class UiVertexDataLayerVersion;
	class UiVertexDataLayerVersionListInternal;

	class UiVertexDataLayerVersionList
	{
	public:
		UiVertexDataLayerVersionList(UiVertexDataLayerVersionListInternal& ref);
		~UiVertexDataLayerVersionList();

		UiVertexDataLayerVersionList& operator=(const UiVertexDataLayerVersionList&) noexcept = delete;

		UiVertexDataLayerVersion GetLayersVersion(size_t versionIndex);

		const UiVertexDataLayerVersion GetLayersVersion(size_t versionIndex) const;

	private:
		UiVertexDataLayerVersionListInternal& _internal;
	};
}
